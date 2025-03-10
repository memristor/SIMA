                                                                                    #############################################################
                                                                                    Prototip za kontrolu dynamixel motora preko esp-now protokola
                                                                                    #############################################################


Jedan esp salje paket sa kontrolnim signalom <<esp-host.ino>> na zadate mac adrese.
Ove adrese je potrebno ocitati za svaki esp32 ponasaob i upisati ga.
Svaki slave esp prima paket i pocinje sa izvrsavanjem dijela koda koji kontrolise motore.
