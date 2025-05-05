extern volatile bool endFlag;


void setupMotors() {
  DXL_SERIAL.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
  dxl.begin(BAUD_RATE);
  dxl.setPortProtocolVersion(PROTOCOL);

  int br = 0;

  for (int i = 0; i < DXL_ID_CNT; i++) {
    if (dxl.ping(DXL_ID_LIST[i])) {
      br++;
    }
  }
  if (br == DXL_ID_CNT) {
    Serial.println("Successfully connected to motors!");
  } else {
    Serial.println("Failed to connect to motors");
    while (1)
      ;
  }

  dxl.torqueOff(ID1);
  dxl.setOperatingMode(ID1, OP_EXTENDED_POSITION);
  dxl.torqueOn(ID1);

  dxl.torqueOff(ID2);
  dxl.setOperatingMode(ID2, OP_EXTENDED_POSITION);
  dxl.torqueOn(ID2);

  Serial.println("Motors set to Extended Position Control Mode.");
}

void changeVelocity(int32_t vel1, int32_t vel2) {
  sw_data_vel[0].goal_velocity = vel1;
  sw_data_vel[1].goal_velocity = vel2;

  sw_infos_vel.is_info_changed = true;

  if (dxl.syncWrite(&sw_infos_vel) == true) {
    Serial.println("[SyncWrite] Success");
    for (int i = 0; i < sw_infos_vel.xel_count; i++) {
      Serial.print("  ID: ");
      Serial.println(sw_infos_vel.p_xels[i].id);
      Serial.print("\t Goal Velocity: ");
      Serial.println(sw_data_vel[i].goal_velocity);
    }
  } else {
    Serial.print("[SyncWrite] Fail, Lib error code: ");
    Serial.print(dxl.getLastLibErrCode());
  }
}

void changeAcceleration(int32_t acc1, int32_t acc2) {
  sw_data_acc[0].goal_acceleration = acc1;
  sw_data_acc[1].goal_acceleration = acc2;

  sw_infos_acc.is_info_changed = true;

  if (dxl.syncWrite(&sw_infos_acc) == true) {
    Serial.println("[SyncWrite] Success");
    for (int i = 0; i < sw_infos_acc.xel_count; i++) {
      Serial.print("  ID: ");
      Serial.println(sw_infos_acc.p_xels[i].id);
      Serial.print("\t Goal Acceleration: ");
      Serial.println(sw_data_acc[i].goal_acceleration);
    }
  } else {
    Serial.print("[SyncWrite] Fail, Lib error code: ");
    Serial.print(dxl.getLastLibErrCode());
  }
}

void moveMotorsMM(double cm1, double cm2) {

   if (endFlag) {

      dxl.torqueOff(ID1);
      dxl.torqueOff(ID2);
      return;
    }

  const double TICKS_PER_MM = 4095.0 / 265.485;

  int32_t offset1 = static_cast<int32_t>(cm1 * TICKS_PER_MM);
  int32_t offset2 = static_cast<int32_t>(cm2 * TICKS_PER_MM);

  Serial.println(offset1);
  Serial.println(offset2);

  if (!readPosition(pos)) {
    Serial.println("Failed to get current positions. Aborting move.");
    return;
  }

  Serial.print("\t Pos before moving: ");
  Serial.print(pos[0]);
  Serial.print(" ");
  Serial.println(pos[1]);

  int32_t goal_positions[2] = { pos[0] + offset1, pos[1] - offset2 };

  sw_data_pos[0].goal_position = goal_positions[0];
  sw_data_pos[1].goal_position = goal_positions[1];

  sw_infos_pos.is_info_changed = true;

  if (dxl.syncWrite(&sw_infos_pos)) {
    Serial.println("[SyncWrite - Position] Success");
    for (int i = 0; i < sw_infos_pos.xel_count; i++) {
      Serial.print("  ID: ");
      Serial.println(sw_infos_pos.p_xels[i].id);
      Serial.print("\t Goal Position: ");
      Serial.println(sw_data_pos[i].goal_position);
    }
  } else {
    Serial.print("[SyncWrite - Position] Fail, Lib error code: ");
    Serial.println(dxl.getLastLibErrCode());
    return;
  }

  bool stanje = false;
  int32_t remaining[2];
  int32_t velocity[2];

  Serial.println("Waiting for motors to reach target...");
  while (abs(pos[0] - goal_positions[0]) > 20 || abs(pos[1] - goal_positions[1]) > 20) {

    readPosition(pos);

    if (endFlag) {
      changeVelocity(0, 0);
      readPosition(pos);
      sw_data_pos[0].goal_position = pos[0];
      sw_data_pos[1].goal_position = pos[1];

      sw_infos_pos.is_info_changed = true;
      dxl.syncWrite(&sw_infos_pos);

      dxl.torqueOff(ID1);
      dxl.torqueOff(ID2);

      Serial.println("Timer expired or stop condition met, stopping motors...");

      break;
    }

    if (readSensors() && !stanje) {
      changeAcceleration(MAX, MAX);
      changeVelocity(0, 0);
      readPosition(pos);

      remaining[0] = goal_positions[0] - pos[0];
      remaining[1] = goal_positions[1] - pos[1];

      Serial.print("REMAINING POSITION: ");
      Serial.println(remaining[0]);
      Serial.print(remaining[1]);

      sw_data_pos[0].goal_position = pos[0];
      sw_data_pos[1].goal_position = pos[1];

      sw_infos_pos.is_info_changed = true;

      dxl.syncWrite(&sw_infos_pos);

      stanje = true;
    } else if (!readSensors() && stanje) {
      changeAcceleration(MIN, MIN);
      changeVelocity(MAX, MAX);

      sw_data_pos[0].goal_position = pos[0] + remaining[0];
      sw_data_pos[1].goal_position = pos[1] + remaining[1];

      sw_infos_pos.is_info_changed = true;

      dxl.syncWrite(&sw_infos_pos);

      stanje = false;
    }

    delay(10);
  }

  if (endFlag) {
    Serial.println("Motors stopped due to timer or stop condition.");
  } else {
    Serial.print("\t Pos after moving: ");
    Serial.print(pos[0]);
    Serial.print(" ");
    Serial.println(pos[1]);
    Serial.println("Motors moved successfully!");
  }
}


void rotateMotors(double angle_deg) {
  const double WHEEL_DISTANCE_MM = 95;  //wheel sep
  const double TICKS_PER_MM = 4095.0 / 265.485;

  double arc_mm = (PI * WHEEL_DISTANCE_MM * angle_deg) / 360.0;

  moveMotorsMM(arc_mm, -arc_mm);
}


void resetMotors() {
  Serial.println("Resetting motor positions...");

  if (!readPosition(pos)) {
    Serial.println("Failed to get current positions. Aborting move.");
    return;
  }

  int32_t goal_positions[2] = { 0, 0 };

  sw_data_pos[0].goal_position = goal_positions[0];
  sw_data_pos[1].goal_position = goal_positions[1];

  sw_infos_pos.is_info_changed = true;

  // Perform sync write
  if (dxl.syncWrite(&sw_infos_pos)) {
    Serial.println("[SyncWrite - Position] Success");
    for (int i = 0; i < sw_infos_pos.xel_count; i++) {
      Serial.print("  ID: ");
      Serial.println(sw_infos_pos.p_xels[i].id);
      Serial.print("\t Goal Position: ");
      Serial.println(sw_data_pos[i].goal_position);
    }
  } else {
    Serial.print("[SyncWrite - Position] Fail, Lib error code: ");
    Serial.println(dxl.getLastLibErrCode());
    return;
  }

  Serial.println("Waiting for motors to reach target...");
  while (true) {
    if (!readPosition(pos)) {
      Serial.println("Failed to read current positions during wait.");
      continue;
    }

    if (abs(pos[0] - goal_positions[0]) <= 20 && abs(pos[1] - goal_positions[1]) <= 20) {
      break;
    }

    delay(10);
  }

  Serial.println("Motors reset successfully!");
}
