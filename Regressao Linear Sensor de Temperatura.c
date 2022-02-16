// Biblioteca LCD
#include <LiquidCrystal.h>

// Inicializa a biblioteca LCD
LiquidCrystal lcd (12, 11, 5, 4, 3, 2);

int Atencao = 10, Critica = 13, Normal = 8;
int limitNormal = 1, limitAttention = 3, SensorTempPino = 0;

float time_vetor[4], temp_vetor[4], M = 0.0, B = 0.0, alert;
float *time_ptr = time_vetor;
float *temp_ptr = temp_vetor;


void pipelineVetor(float *vetor){
	vetor[0] = vetor[1];
  	vetor[1] = vetor[2];
  	vetor[2] = vetor[3];
  	vetor[3] = 0;
}


void
InsereTemp (float time[4], float temp[4], int flag)
{
  int cont = 0;
  int seg = 0;
  int vetor_ok = 0;

  while (vetor_ok == 0)
    {

      static unsigned long tempoAntes = 0;
      int tempo = millis ();
      if (tempo - tempoAntes >= 1000)
	{
      
      if (flag == 0){
       tempoAntes = tempo;
       cont = seg ;
	   seg++;
       

      }
       else if(flag==1){
       tempoAntes = tempo;
       cont = 3;
	   seg = 4;
       
  }
  
	  if (cont < 4)
	    {
        
        if(flag == 0){
	      int SensorTempTensao = analogRead (SensorTempPino);
	      float Tensao = SensorTempTensao * 5;
	      Tensao /= 1024;
	      float TemperaturaC = (Tensao - 0.5) * 100;
        
	      temp[cont] = TemperaturaC;        
	      time[cont] = seg;

	      lcd.clear ();
	      lcd.setCursor (0, 0);
	      lcd.print ("Temp: ");
	      lcd.setCursor (7, 0);
	      lcd.print (temp[cont]);

	      lcd.setCursor (0, 1);
	      lcd.print ("Tempo: ");
	      lcd.setCursor (7, 1);
	      lcd.print (seg);
        }else if(flag == 1){
        
          int SensorTempTensao = analogRead (SensorTempPino);
	      float Tensao = SensorTempTensao * 5;
	      Tensao /= 1024;
	      float TemperaturaC = (Tensao - 0.5) * 100;
        
	      temp[cont] = TemperaturaC;        
	      time[cont] = seg;
          lcd.clear ();
	      lcd.setCursor (0, 0);
	      lcd.print ("Temp: ");
	      lcd.setCursor (7, 0);
	      lcd.print (temp[cont]);

	      lcd.setCursor (0, 1);
	      lcd.print ("Tempo: ");
	      lcd.setCursor (7, 1);
	      lcd.print (seg);
          delay (2000);
          
	      float *time_ptr = time;
	      float *temp_ptr = temp;
          
          vetor_ok = 1;
        
        }
        
	    }else{
	      vetor_ok = 1;
	      float *time_ptr = time;
	      float *temp_ptr = temp;
	  }
	}
  }
}

void
Alert (float alert)
{
  if (M > limitAttention)
    {

      digitalWrite (Critica, HIGH);
      delay (2000);
      digitalWrite (Critica, LOW);

  }else if(M >= limitNormal && M <= limitAttention) {
    
 	  digitalWrite (Atencao, HIGH);
      delay (2000);
      digitalWrite (Atencao, LOW);
  }

  else if (M < limitAttention)
    {

 	  digitalWrite (Normal, HIGH);
      delay (2000);
      digitalWrite (Normal, LOW);
    }


}

void
ImprimeVetor (float *x, float *y)
{
  int i = 0;
  lcd.setCursor (0, 0);
  lcd.print ("T[");
  for (i = 0; i < 4; i++)
    {
      lcd.print (x[i], 1);
      if (i < 3)
	{
	  lcd.print ("|");
	}
    }
  lcd.print ("]");
  lcd.setCursor (0, 1);
  lcd.print ("t[");
  for (i = 0; i < 4; i++)
    {
      lcd.print (y[i], 1);
      if (i < 3)
	{
	  lcd.print ("|");
	}
    }
  lcd.print ("]");
  delay (2000);
}


void
regLinear (float x[4], float y[4], float alert)
{
  int i;
  float x1 = 0.0, y1 = 0.0, xy = 0.0, x2 = 0.0;
  for (int i = 0; i < 4; i++)
    {
      x1 = x1 + x[i];
      y1 = y1 + y[i];
      xy = xy + x[i] * y[i];
      x2 = x2 + x[i] * x[i];
    }
  float J = 0.0;
  J = 4 * x2 - (x1 * x1);
  if (J != 0.0)
    {
      M = ((4 * xy) - (x1 * y1)) / J;
      B = ((y1 * x2) - (x1 * xy)) / J;
    }

  alert = M;ca
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("f(x)=");
  lcd.print (M);
  lcd.print ("x+");
  lcd.print (B);
  delay (2000);

}

void
setup ()
{
  
  pipelineVetor(time_ptr);
  pipelineVetor(temp_ptr);
  lcd.begin (16, 2);
  pinMode (Critica, OUTPUT);
  pinMode (Normal, OUTPUT);
  pinMode (Atencao, OUTPUT);

  InsereTemp (time_ptr, temp_ptr, 0);
  ImprimeVetor (time_ptr, temp_ptr);
  regLinear (time_ptr, temp_ptr, alert);
  Alert (alert);

  delay (2000);
  lcd.clear ();

}

void
loop ()
{
  InsereTemp (time_ptr, temp_ptr, 1);
  ImprimeVetor (time_ptr, temp_ptr);
  regLinear (time_ptr, temp_ptr, alert);
  Alert (alert);

  delay (2000);
  lcd.clear ();
}
