//+------------------------------------------------------------------+
//|                                              AcumulacionWyckoff  |
//|                        Creado por ChatGPT para Franco Ramirez :) |
//+------------------------------------------------------------------+
#property indicator_buffers 1
#property indicator_plots 1
#property indicator_color1 0
#property indicator_label1 "Wyckoff's Accumulations"

#include "DebuggerConsoleComments.mqh"

input int AcumulacionPeriod = 30;       // Periodo para calcular varianza y RSI
input double VarianzaMaxima = 100.0;     // Varianza máxima permitida para considerar acumulación
input double RSIMargen = 10.0;           // Margen alrededor de 50 para RSI (ej: 40-60)
input int MinimoVelasZona = 5;           // Mínimo de velas continuas que cumplan para dibujar zona

color ColorAcumulacion = clrRoyalBlue;   // Color del rectángulo

double priceBuffer[];
datetime last_candle_time = 0;

DebuggerConsoleComments* console = DebuggerConsoleComments::GetInstance();

//+------------------------------------------------------------------+
int OnInit()
  {
   SetIndexBuffer(0, priceBuffer);
   return(INIT_SUCCEEDED);
  }

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

// Check if there is a new candle.
   if(time[rates_total - 1] != last_candle_time)
     {
      last_candle_time = time[rates_total - 1]; // Update last candle time.

      // console.Log("Event on new candle");
      // Call your custom function for new candle logic.
      OnNewCandle(rates_total,
                  prev_calculated,
                  time,
                  open,
                  high,
                  low,
                  close,
                  tick_volume,
                  volume,
                  spread
                 ); // this work
     }

   return(rates_total);
  }

//+------------------------------------------------------------------+
void OnNewCandle(const int rates_total,
                 const int prev_calculated,
                 const datetime &time[],
                 const double &open[],
                 const double &high[],
                 const double &low[],
                 const double &close[],
                 const long &tick_volume[],
                 const long &volume[],
                 const int &spread[]) // custom only for OnCalculated
  {
// Get RSI
   int handleRSI = iRSI(NULL, 0, AcumulacionPeriod, PRICE_CLOSE);
   if(handleRSI == INVALID_HANDLE)
     {
      console.Log("RSI Handler is Invalid");
      return;
     }

   ObjectsDeleteAll(0, "Zone", 0);

   double rsiBuffer[];
   ArraySetAsSeries(rsiBuffer, true);
   CopyBuffer(handleRSI, 0, 0, rates_total, rsiBuffer);
   IndicatorRelease(handleRSI);

   int start = AcumulacionPeriod;
   int zonaInicio = -1;
   bool enZona = false;

   for(int i = start; i < rates_total; i++)
     {
      // Calc varianza
      double media = 0;
      for(int j = 0; j < AcumulacionPeriod; j++)
         media += close[i - j];
      media /= AcumulacionPeriod;

      double varianza = 0;
      for(int j = 0; j < AcumulacionPeriod; j++)
         varianza += MathPow(close[i - j] - media, 2);
      varianza /= AcumulacionPeriod;

      // Main Conditions
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
  }

//+------------------------------------------------------------------+
void DibujarRectangulo(const datetime &time[], const double &low[], const double &high[], int startIndex, int endIndex)
  {
   MqlDateTime startMDT;
   if(!TimeToStruct(startIndex, startMDT))
      console.Log("Failed converting time to strcut");

   string nombre = "Zone" + TimeToString(datetime(startIndex), TIME_DATE|TIME_MINUTES|TIME_SECONDS) + "_" + IntegerToString(startIndex);
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

   console.Log("Total Object:"+(string)ObjectsTotal(0,0,OBJ_RECTANGLE));
  }
//+------------------------------------------------------------------+
