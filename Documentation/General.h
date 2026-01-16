/*
Set a bit

 bit_fld |= (1 << n)

Clear a bit

 bit_fld &= ~(1 << n)

Toggle a bit

 bit_fld ^= (1 << n)

Test a bit

 bit_fld & (1 << n)


 */ 
/* OLD INT0

	tmpBit=0;
	
	tmpBit = PIND & 1 << 6; //Data bit 
	
	if(tmpBit == 0 & pkCurrentBit == 0)
	{
		reti();
		return;
	}		
	else
		PORTD &= ~1 << 7;
		
	PORTC ^= 1<<0;
	
	switch(pkCurrentBit)
	{
		case 0:
			if(tmpBit > 0)
			{
				pkCurrentBit = 0;
				pkData = 0;
				oddEvenCheck = 0;
				reti();
				return;
			}					
		break;
		
		case 1:
			if(tmpBit == 0)
			{
				pkData |= 1 << 0;			
				oddEvenCheck++;
			}				
			else
				pkData &= ~(1 << 0);
		break;
		
		case 2:
			if(tmpBit == 0)
			{
				pkData |= 1 << 0;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 1);
		break;
		
		case 3:
			if(tmpBit == 0)
			{
				pkData |= 1 << 2;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 2);
		break;
		
		case 4:
			if(tmpBit == 0)
			{
				pkData |= 1 << 3;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 3);
		break;
		
		case 5:
			if(tmpBit == 0)
			{
				pkData |= 1 << 4;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 4);
		break;
		
		case 6:
			if(tmpBit == 0)
			{
				pkData |= 1 << 5;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 5);
		break;
		
		case 7:
			if(tmpBit == 0)
			{
				pkData |= 1 << 6;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 6);
		break;
		
		case 8:
			if(tmpBit == 0)
			{
				pkData |= 1 << 7;			
				oddEvenCheck++;
			}		
			else
				pkData &= ~(1 << 7);
		break;

		case 9: // Parity Check 
			
			if(tmpBit == 0) // parity bit is 1, add to bitones
			{
				oddEvenCheck++;
			}		
			
			if(oddEvenCheck % 2 == 0)
			{	// Some error happened, odd parity check failed (even number)
				pkCurrentBit = 0;
				pkData = 0;
				oddEvenCheck = 0;
				reti();
				return;	
			}
			else
			{ // ALL OK 

				PORTD |= 1 << 7;
			}				
		break;
		
		case 10: // stop bit
			if(tmpBit == 0)
			{	
				PORTD |= 1 << 7;
				
			}
			else
			{
				// not a valid data packet
				pkCurrentBit = 0;
				pkData = 0;
				oddEvenCheck = 0;
				reti();
				return;	
			}						
		break;
		
		case 11:
			pkCurrentBit = 0;
			if(pkData != 253)
				PORTD |= 1 << 7;
		break;
		
		default:
		break;			
	}		
	

	
	pkCurrentBit++;
	reti();
	*/
	/*
	BACKUP OLD INT ADC INTERRUPT
	
	ISR(ADC_vect)
{
	Voltage  = (float)ADCH * (float)VPERADC; //Voltage in to ADC
	if(Sektion.SektionUsed[Sektion.ADCchannel])
	{
	//itoa(i, Loading, 10);

		if(Voltage > 4) //DRIFT
		  {	
				Sektion.OK			|= 1 << Sektion.ADCchannel;
				Sektion.Alarm		&= ~1 << Sektion.ADCchannel;
				Sektion.Sabotage	&= ~1 << Sektion.ADCchannel;
			  sprintf(AlarmText, "OK Sect: %i", Sektion.ADCchannel);
				PORTC |= 1<<0;  //Green on
				PORTD &= ~1<<7; //Red off
		  }		
		else if(Voltage>3 && Voltage<4) //LARM
		  {
				Sektion.Alarm		|= 1 << Sektion.ADCchannel;
				Sektion.OK			&= ~1 << Sektion.ADCchannel;
				Sektion.Sabotage	&= ~1 << Sektion.ADCchannel;
				#ifndef DEBUG
					sprintf(AlarmText, "Alarm in section: %i\n", Sektion.ADCchannel);
				#else
					sprintf(AlarmText, "Alarm:%i Voltage:%i", Sektion.ADCchannel, Voltage);
				#endif
				PORTC &= ~1<<0;  //Green 
				PORTD |= 1<<7; //Red 
				
			
			}
			else //SAB
			{
				Sektion.Sabotage	|= 1 << Sektion.ADCchannel;
				Sektion.OK			&= ~1 << Sektion.ADCchannel;
				Sektion.Alarm		&= ~1 << Sektion.ADCchannel;
		
			
				#ifndef DEBUG
					sprintf(SabotageText, "Sabotage section:%i\n", Sektion.ADCchannel);
				#else
					sprintf(SabotageText, "Sab:%i Voltage:%f", Sektion.ADCchannel, Voltage);
				#endif
			 	
			  	PORTC |= 1<<0;  //Green 
				PORTD |= 1<<7; //Red 
				
				
		  }		
		lcd_clrscr();
		lcd_puts(AlarmText);
		lcd_putc('\n');
		lcd_puts(SabotageText);
		_delay_ms(1000); 
	}		
	
	if(Sektion.ADCchannel < 7)
		Sektion.ADCchannel ++;
	else
	{
		Sektion.ADCchannel = 0;
	}		


	SFIOR |= (1 << ACME);  //Set ACME bit in ADCSRB (Enable ADC Multiplexing)
	ADCSRA &= ~1 << ADEN;	//Clear ADEN bit in ADCSRA (TURN ADC OFF)


	switch(Sektion.ADCchannel)
	{

	case 0:
		ADMUX |= (0 << MUX2) | (0 << MUX1) | (0 << MUX0); //enable ADC0 - 000
		break;
	case 1:
		ADMUX |= (0 << MUX2) | (0 << MUX1) | (1 << MUX0); //enable ADC1 - 001
		break;
	case 2:
		ADMUX |= (0 << MUX2) | (1 << MUX1) | (0 << MUX0); //enable ADC2 - 010
		break;
	case 3:
		ADMUX |= (0 << MUX2) | (1 << MUX1) | (1 << MUX0); //enable ADC3 - 100
		break;
	case 4:
		ADMUX |= (1 << MUX2) | (0 << MUX1) | (0 << MUX0); //enable ADC0 - 000
		break;
	case 5:
		ADMUX |= (1 << MUX2) | (0 << MUX1) | (1 << MUX0); //enable ADC1 - 001
		break;
	case 6:
		ADMUX |= (1 << MUX2) | (1 << MUX1) | (0 << MUX0); //enable ADC2 - 010
		break;
	case 7:
		ADMUX |= (1 << MUX2) | (1 << MUX1) | (1 << MUX0); //enable ADC3 - 100
		break;
	}
	
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // Set ADC prescalar to 64 - 125KHz sample rate @ 8MHz


	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	SFIOR |= (1 << ACME);  //Set ACME bit in ADCSRB (Enable ADC Multiplexing)
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

	ADCSRA |= (1 << ADEN);  // Enable ADC
   	ADCSRA |= (1 << ADIE);	//Enable ADC interrupt
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
	
	Voltage = 0;
	
	*/