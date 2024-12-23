int select = 0; // Variable de sélection
int led = 0; // Variable pour allumage des leds

void step_by_step_mode();
void auto_step_mode();
void all_on_mode();
void led_clear();

void main(){
     TRISA = 0b00000010; // Ports A en sortie sauf RA1
     PORTA = 0b11001000 ; // mettre les sorties A = 0 sauf RA6, RA7, RA3
     TRISB = 0b00000001; // Ports B en sortie sauf RB0
     PORTB = 0 ; // mettre les sorties B = 0
     pcon.OSCF = 1; // configure le pic sur 4 mhz
     CMCON = 0b00000111; // desactive les comparateurs
     INTCON = 0b10010000; //Active les interruptions globales et externes
     OPTION_REG=0b01000000; //Configure les interruptions sur un flan montant
     
     while(1){
              if(RB0_bit == 0){
                         delay_ms(600);
                         if(RB0_bit == 0){
                                    select++; // Incrémente la sélection
                                    switch(select){ // sélecteur
                                                   case 1:  // Pour chaque cas, affichage du mode et petit délai
                                                        led_clear(); // Voir definition
                                                        RA3_bit = 0;
                                                        RA6_bit = 1;
                                                        RA7_bit = 1;
                                                        delay_ms(300);
                                                        step_by_step_mode(); // Voir definition
                                                        break;
                                                   case 2:
                                                        led_clear();
                                                        RA3_bit = 1;
                                                        RA6_bit = 0;
                                                        RA7_bit = 1;
                                                        delay_ms(300);
                                                        auto_step_mode(); // Voir definition
                                                        break;
                                                   case 3:
                                                        led_clear();
                                                        RA3_bit = 1;
                                                        RA6_bit = 1;
                                                        RA7_bit = 0;
                                                        delay_ms(300);
                                                        all_on_mode(); // Voir definition
                                                        break;
                                                   default: // Cas par défaut --> réinitialise la sélection
                                                           led_clear();
                                                           RA3_bit = 1;
                                                           RA6_bit = 1;
                                                           RA7_bit = 1;
                                                           select = 0;
                                    }
                         }
              }
     }
}

void step_by_step_mode(){  // Mode pas par pas manuel
     led = 0;
     while(1){
              PORTB = led;
              if(led == 0){RA0_bit = 1;led = 1;}
              if(led == 256){led = 0;}
              if(RB0_bit == 0){
                         led = led * 2; // Avance dans la suite
                         RA0_bit = 0;
                         delay_ms(600);
                         //if(RB0_bit == 0){break;} // Quitte le mode si appuis long
              }
     }
}

void auto_step_mode(){ // Mode automatique --> pareil que le pas par pas mais sans interaction nécessaire
     led = 0;
     while(1){
              led = led * 2;
              PORTB = led;
              if(led == 0){RA0_bit = 1;led = 1;}
              if(led == 256){led = 0;}
              delay_ms(250);
              //if(RB0_bit == 0){break;}
              RA0_bit = 0;
     }
}

void all_on_mode(){ // Mode tout allumé --> pareil que automatique mais sans délai
     while(1){
              led = led * 2;
              PORTB = led;
              if(led == 0){RA0_bit = 1;led = 1;}
              if(led == 256){led = 0;}
              delay_ms(1);
              //if(RB0_bit == 0){break;}
              RA0_bit = 0;
     }
}

void led_clear(){ // Eteind les 8 leds
     RA0_bit = 0;
     PORTB = 0;
}

void interrupt(){
     led_clear();
     INTCON.INTF = 0;
}