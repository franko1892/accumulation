//+------------------------------------------------------------------+
//|                                              AcumulacionWyckoff  |
//|                        Creado por ChatGPT para ti :)             |
//+------------------------------------------------------------------+
#property indicator_chart_window
#property strict

#include <Trade\Trade.mqh>

input int AcumulacionPeriod = 30;       // Periodo para calcular varianza y RSI
input double VarianzaMaxima = 100.0;     // Varianza máxima permitida para considerar acumulación
input double RSIMargen = 10.0;           // Margen alrededor de 50 para RSI (ej: 40-60)
input int MinimoVelasZona = 5;           // Mínimo de velas continuas que cumplan para dibujar zona

color ColorAcumulacion = clrRoyalBlue;   // Color del rectángulo

double priceBuffer[];

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int OnInit()
  {
   SetIndexBuffer(0, priceBuffer);
   return(INIT_SUCCEEDED);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
int OnCalculate(const int rates_total,
                const int prev_calculated,
                const datetime &time[],
                const double &open[],
                const double &high[],
                const double &low[],
                const double &close[],
                const long &tick_volume[],
                const long &volume[],
                const int &spread[])
  {
   if(rates_total < AcumulacionPeriod)
      return(0);

// Obtenemos el RSI
   int handleRSI = iRSI(NULL, 0, AcumulacionPeriod, PRICE_CLOSE);
   if(handleRSI == INVALID_HANDLE)
      return(0);

   double rsiBuffer[];
   ArraySetAsSeries(rsiBuffer, true);
   CopyBuffer(handleRSI, 0, 0, rates_total, rsiBuffer);
   IndicatorRelease(handleRSI);

   int start = AcumulacionPeriod;
   int zonaInicio = -1;
   bool enZona = false;

   for(int i = start; i < rates_total; i++)
     {
      // Calcular varianza
      double media = 0;
      for(int j = 0; j < AcumulacionPeriod; j++)
         media += close[i - j];
      media /= AcumulacionPeriod;

      double varianza = 0;
      for(int j = 0; j < AcumulacionPeriod; j++)
         varianza += MathPow(close[i - j] - media, 2);
      varianza /= AcumulacionPeriod;

      // Condiciones de acumulación
      bool bajaVarianza = varianza <= VarianzaMaxima;
      bool rsiCercano = MathAbs(rsiBuffer[i] - 50) <= RSIMargen;

      if(bajaVarianza && rsiCercano)
        {
         if(!enZona)
           {
            zonaInicio = i;
            enZona = true;
           }
        }
      else
        {
         if(enZona)
           {
            if((zonaInicio - i) <= -MinimoVelasZona)
               DibujarRectangulo(time, low, high, zonaInicio, i-1);
            enZona = false;
           }
        }
     }

   return(rates_total);
  }

//+------------------------------------------------------------------+
//| Dibuja un rectángulo entre dos velas                             |
//+------------------------------------------------------------------+
void DibujarRectangulo(const datetime &time[], const double &low[], const double &high[], int startIndex, int endIndex)
  {
   string nombre = "ZonaAcumulacion_" + IntegerToString(TimeToStructSimple(time[startIndex]).day) + "_" + IntegerToString(startIndex);
   double precioMax = high[startIndex];
   double precioMin = low[startIndex];

   for(int i = startIndex; i <= endIndex; i++)
     {
      if(high[i] > precioMax)
         precioMax = high[i];
      if(low[i] < precioMin)
         precioMin = low[i];
     }

   datetime tiempoInicio = time[endIndex];
   datetime tiempoFin = time[startIndex];

   if(ObjectFind(0, nombre) != -1)
      ObjectDelete(0, nombre);

   ObjectCreate(0, nombre, OBJ_RECTANGLE, 0, tiempoInicio, precioMax, tiempoFin, precioMin);
   ObjectSetInteger(0, nombre, OBJPROP_COLOR, ColorAcumulacion);
   ObjectSetInteger(0, nombre, OBJPROP_STYLE, STYLE_DOT);
   ObjectSetInteger(0, nombre, OBJPROP_WIDTH, 2);
   ObjectSetInteger(0, nombre, OBJPROP_BACK, true);
  }

//+------------------------------------------------------------------+
//| Convertir datetime en estructura simple                         |
//+------------------------------------------------------------------+
MqlDateTime TimeToStructSimple(datetime tiempo)
  {
   MqlDateTime ts;
   TimeToStruct(tiempo, ts);
   return ts;
  }

//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
