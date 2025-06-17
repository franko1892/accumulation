//+------------------------------------------------------------------------+
//|                                                    Smartflow.mq5       |
//|                                  Copyright 2025, Franco Ramirez.       |
//|                       https://www.instagram.com/frankoramirez92/       |
//| Prohibida su reproducción total o parcial sin consentimiento           |
//| de su creador. Estrategia basada en SmartMoney Concept                 |
//|A veces las personas de las que nadie espera nada, son las que logran   |
//|lo que nadie puede imaginar                                             |
//+------------------------------------------------------------------------+

#property copyright "Copyright 2025, EA desarrollado por Franco Ramirez."
#property version   "1.00"

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+

#include <Trade\Trade.mqh>

CTrade trade;
// Parámetros de entrada para sesiones
input group "Sesiones de Trading"
input bool     OperarLondres = true;         // Operar Londres
input bool     OperarNY = true;              // Operar Nueva York
input bool     OperarTokyo = true;           // Operar Tokyo
input bool     EntradaPersonalizada = false; // Entrada Personalizada

// Horarios de sesiones (GMT)
input group "Horarios de Sesiones"
input int      HoraInicioLondres = 8;        // Hora inicio Londres
input int      HoraCierreLondres = 16;       // Hora cierre Londres
input int      HoraInicioNY = 13;            // Hora inicio Nueva York
input int      HoraCierreNY = 21;            // Hora cierre Nueva York
input int      HoraInicioTokyo = 0;          // Hora inicio Tokyo
input int      HoraCierreTokyo = 8;          // Hora cierre Tokyo
input int      HoraInicioPersonal = 0;       // Hora inicio Personal
input int      HoraCierrePersonal = 0;       // Hora cierre Personal
input int      GMTOffset = 0;           
input bool     Horadeverano = false;     // Diferencia horaria GMT

// Parámetros de trading
input group "Configuración de Trading"
input bool     Cuentafondeo = true;          // Cuenta Fondeo
input bool     OperarRiesgoManual = true;     // Operar Riesgo Manual
input double   RiesgoPorTrade = 1.0;         // Riesgo por trade (%)
input double   StopLossPoints = 100;         // Stop Loss en puntos
input int      RiesgoRecompensa = 3;         // Multiplicador de Take Profit
input int      MaxTradesPorSesion = 3;       // Máximo trades por sesión
input double   BreakevenRatio = 1.5;         // Ratio para Break Even
input double   MaxSpreadPermitido = 20;  
input bool     OperarRiesgoautomático = false;      // Riesgo automático
input int DistanciaMaximaAlPuntoEntrada = 50;   // Máxima distancia del precio actual al punto medio (en puntos)
input int DistanciaMinimaEntreOrdenes = 100;  // Distancia mínima entre órdenes limit (en puntos)
input int StopLossBuffer = 10;               // Buffer para Stop Loss en puntos

// Parámetros Smart Money
input group "Configuración Smart Money"
input bool     OperarImbalance = true;       // Operar Imbalances
input bool     OperarOrderblock = true;       // Operar Orderblocks
input bool     OperarLiquidezPrevia = true;  // Operar Liquidez Previa
input bool     OperarMaxMin = true;          // Operar Máximos y Mínimos
input int      MaxVelasAnalisis = 500;       // Máximo velas a analizar
input int      TamanoMinImbalance = 10;      // Tamaño mínimo imbalance
input int      DistanciaUnirImbalance = 5;   // Distancia para unir imbalances
input int      TamanoIBMinparaOB = 15;             // Tamaño mínimo OrderBlock
input int      DistanciaUnirOB = 5;          // Distancia para unir OrderBlocks
input int      TamanoMinOB = 10;
input int      TamanoMaximoOB = 120;         // Tamaño máximo OrderBlock
input int      DistanciaImbalanceOB = 20;    // Distancia Imbalance-OB
input int      DistanciaOBMaxMin = 30;       // Distancia OB a Max/Min
input int      DistanciaHighLow = 50;        // Distancia máxima High/Low
input color    ColorImbalanceAlcista = clrBlue;     // Color Imbalance Alcista
input color    ColorImbalanceBajista = clrMagenta;  // Color Imbalance Bajista
input color    ColorImbalanceAlcistaMitigado = clrDarkGreen; // Color para imbalances alcistas mitigados
input color    ColorImbalanceBajistaMitigado = clrMaroon; // Color para imbalances bajistas mitigados
input color    ColorOBAlcista = clrGreen;           // Color OB Alcista
input color    ColorOBBajista = clrRed;             // Color OB Bajista
input color    ColorMaximo = clrGold;               // Color línea máximo
input color    ColorMinimo = clrOrange;             // Color línea mínimo

// Confirmaciones OrderBlocks
input group "Confirmaciones OrderBlocks"
input bool     Confluencia = true;           // Activar confirmación por confluencia
input bool     BreakofStructure = true;      // Activar confirmación por break of structure
input bool     LiqPrevOB = true;             // Activar confirmación por liquidez previa
input bool     Induccion = true;             // Activar confirmación por inducción
input bool     PatronMW = true;              // Activar confirmación por patrón MW
input int      DiasConfluencia = 30;         // Días para comprobar confluencia
input int      ToquesMinConfluencia = 2;     // Número mínimo de toques para confluencia

// Días operativos
input group "Días de Trading"
input bool     Lunes = true;                 // Operar Lunes
input bool     Martes = true;                // Operar Martes
input bool     Miercoles = true;             // Operar Miércoles
input bool     Jueves = true;                // Operar Jueves
input bool     Viernes = true;               // Operar Viernes

// Noticias Fundamentales
input group "Noticias Fundamentales - USD (Mensuales)"
input bool     USD_NominasNoAgricolas = false;        // Ignorar Nóminas No Agrícolas
input datetime USD_NominasNoAgricolas_Fecha = D'2024.01.05 13:30';  // Fecha y hora Nóminas No Agrícolas
input bool     USD_IPC = false;                       // Ignorar IPC (Inflación)
input datetime USD_IPC_Fecha = D'2024.01.11 13:30';  // Fecha y hora IPC
input bool     USD_PIB = false;                       // Ignorar PIB
input datetime USD_PIB_Fecha = D'2024.01.25 13:30';  // Fecha y hora PIB
input bool     USD_VentasMinoristas = false;          // Ignorar Ventas Minoristas
input datetime USD_VentasMinoristas_Fecha = D'2024.01.17 13:30';  // Fecha y hora Ventas Minoristas
input bool     USD_ProduccionIndustrial = false;     // Ignorar Producción Industrial
input datetime USD_ProduccionIndustrial_Fecha = D'2024.01.17 14:15';  // Fecha y hora Producción Industrial

input group "Noticias Fundamentales - USD (Semanales)"
input bool     USD_SolicitudesDesempleo = false;     // Ignorar Solicitudes de Desempleo
input datetime USD_SolicitudesDesempleo_Fecha = D'2024.01.04 13:30';  // Fecha y hora Solicitudes Desempleo
input bool     USD_InventariosPetroleo = false;      // Ignorar Inventarios de Petróleo
input datetime USD_InventariosPetroleo_Fecha = D'2024.01.03 15:30';  // Fecha y hora Inventarios Petróleo

input group "Noticias Fundamentales - EUR (Mensuales)"
input bool     EUR_InflacionMensual = false;          // Ignorar Inflación Mensual
input datetime EUR_InflacionMensual_Fecha = D'2024.01.17 10:00';  // Fecha y hora Inflación EUR
input bool     EUR_PIB = false;                       // Ignorar PIB Eurozona
input datetime EUR_PIB_Fecha = D'2024.01.30 10:00';  // Fecha y hora PIB EUR
input bool     EUR_ProduccionIndustrial = false;     // Ignorar Producción Industrial
input datetime EUR_ProduccionIndustrial_Fecha = D'2024.01.15 10:00';  // Fecha y hora Producción Industrial EUR
input bool     EUR_VentasMinoristas = false;          // Ignorar Ventas Minoristas
input datetime EUR_VentasMinoristas_Fecha = D'2024.01.08 10:00';  // Fecha y hora Ventas Minoristas EUR
input bool     EUR_BalanzaComercial = false;          // Ignorar Balanza Comercial
input datetime EUR_BalanzaComercial_Fecha = D'2024.01.16 10:00';  // Fecha y hora Balanza Comercial EUR

input group "Noticias Fundamentales - EUR (Semanales)"
input bool     EUR_ConfianzaConsumidor = false;      // Ignorar Confianza del Consumidor
input datetime EUR_ConfianzaConsumidor_Fecha = D'2024.01.23 10:00';  // Fecha y hora Confianza Consumidor EUR

input group "Noticias Fundamentales - GBP (Mensuales)"
input bool     GBP_InflacionMensual = false;          // Ignorar Inflación Mensual
input datetime GBP_InflacionMensual_Fecha = D'2024.01.17 07:00';  // Fecha y hora Inflación GBP
input bool     GBP_PIB = false;                       // Ignorar PIB Reino Unido
input datetime GBP_PIB_Fecha = D'2024.01.12 07:00';  // Fecha y hora PIB GBP
input bool     GBP_VentasMinoristas = false;          // Ignorar Ventas Minoristas
input datetime GBP_VentasMinoristas_Fecha = D'2024.01.19 07:00';  // Fecha y hora Ventas Minoristas GBP
input bool     GBP_ProduccionIndustrial = false;     // Ignorar Producción Industrial
input datetime GBP_ProduccionIndustrial_Fecha = D'2024.01.12 07:00';  // Fecha y hora Producción Industrial GBP
input bool     GBP_BalanzaComercial = false;          // Ignorar Balanza Comercial
input datetime GBP_BalanzaComercial_Fecha = D'2024.01.12 07:00';  // Fecha y hora Balanza Comercial GBP

input group "Noticias Fundamentales - JPY (Mensuales)"
input bool     JPY_InflacionMensual = false;          // Ignorar Inflación Mensual
input datetime JPY_InflacionMensual_Fecha = D'2024.01.19 23:30';  // Fecha y hora Inflación JPY
input bool     JPY_PIB = false;                       // Ignorar PIB Japón
input datetime JPY_PIB_Fecha = D'2024.01.15 23:50';  // Fecha y hora PIB JPY
input bool     JPY_ProduccionIndustrial = false;     // Ignorar Producción Industrial
input datetime JPY_ProduccionIndustrial_Fecha = D'2024.01.31 23:30';  // Fecha y hora Producción Industrial JPY
input bool     JPY_VentasMinoristas = false;          // Ignorar Ventas Minoristas
input datetime JPY_VentasMinoristas_Fecha = D'2024.01.29 23:50';  // Fecha y hora Ventas Minoristas JPY
input bool     JPY_BalanzaComercial = false;          // Ignorar Balanza Comercial
input datetime JPY_BalanzaComercial_Fecha = D'2024.01.24 23:50';  // Fecha y hora Balanza Comercial JPY

input group "Noticias Fundamentales - CAD (Mensuales)"
input bool     CAD_InflacionMensual = false;          // Ignorar Inflación Mensual
input datetime CAD_InflacionMensual_Fecha = D'2024.01.16 13:30';  // Fecha y hora Inflación CAD
input bool     CAD_PIB = false;                       // Ignorar PIB Canadá
input datetime CAD_PIB_Fecha = D'2024.01.31 13:30';  // Fecha y hora PIB CAD
input bool     CAD_VentasMinoristas = false;          // Ignorar Ventas Minoristas
input datetime CAD_VentasMinoristas_Fecha = D'2024.01.22 13:30';  // Fecha y hora Ventas Minoristas CAD
input bool     CAD_BalanzaComercial = false;          // Ignorar Balanza Comercial
input datetime CAD_BalanzaComercial_Fecha = D'2024.01.09 13:30';  // Fecha y hora Balanza Comercial CAD

input group "Noticias Fundamentales - AUD (Mensuales)"
input bool     AUD_InflacionMensual = false;          // Ignorar Inflación Mensual
input datetime AUD_InflacionMensual_Fecha = D'2024.01.31 00:30';  // Fecha y hora Inflación AUD
input bool     AUD_PIB = false;                       // Ignorar PIB Australia
input datetime AUD_PIB_Fecha = D'2024.01.31 00:30';  // Fecha y hora PIB AUD
input bool     AUD_VentasMinoristas = false;          // Ignorar Ventas Minoristas
input datetime AUD_VentasMinoristas_Fecha = D'2024.01.09 00:30';  // Fecha y hora Ventas Minoristas AUD
input bool     AUD_BalanzaComercial = false;          // Ignorar Balanza Comercial
input datetime AUD_BalanzaComercial_Fecha = D'2024.01.04 00:30';  // Fecha y hora Balanza Comercial AUD

input group "Noticias Fundamentales - CHF (Mensuales)"
input bool     CHF_InflacionMensual = false;          // Ignorar Inflación Mensual
input datetime CHF_InflacionMensual_Fecha = D'2024.01.04 08:30';  // Fecha y hora Inflación CHF
input bool     CHF_PIB = false;                       // Ignorar PIB Suiza
input datetime CHF_PIB_Fecha = D'2024.01.31 08:00';  // Fecha y hora PIB CHF
input bool     CHF_BalanzaComercial = false;          // Ignorar Balanza Comercial
input datetime CHF_BalanzaComercial_Fecha = D'2024.01.22 08:00';  // Fecha y hora Balanza Comercial CHF

input group "Configuración de Zona Horaria"
input int      ZonaHorariaUsuario = -6;               // Zona horaria del usuario (GMT+/-X)
input bool     HorarioVeranoUsuario = false;         // Horario de verano activo en zona del usuario

#define TIPO_SESION    0
#define TIPO_DIARIO    1
#define TIPO_SEMANAL   2

// Estructuras
struct Imbalance {
    string id; // Identificador único tipo IB_ID
    datetime time;
    double high;
    double low;
    int tipo;
    bool unificado;
    double tamano;
    bool mitigado;
    bool operado;
    int vela_inicio;
    int vela_fin;
    datetime tiempo_mitigacion;
    bool operable; // Nuevo campo
    double punto_medio; // Nuevo campo
};

struct OrderBlock {
    string id;                    // Identificador único
    datetime time;
    double high;
    double low;
    int tipo;                     // 0 = Bajista, 1 = Alcista
    bool mitigado;
    double tamano;
    double punto_medio;           // 50% del tamaño del OB
    int vela_inicio;              // Índice de la vela del OB
    bool operable;                // Si es operable
    bool operado;                 // Si ya fue operado
    // Confirmaciones
    bool confluencia;
    bool breakofstructure;
    bool liquidez_previa;
    bool induccion;
    bool patron_mw;
};

struct MaximoMinimo {
    datetime time;
    double maximo;
    double minimo;
    string sesion;
    int tipo;      // Cambiado de string a int
    bool roto;
};

struct LiquidezPrevia {
    datetime time;
    double nivel;
    int tipo;      // Changed from string to int (0 = Baja, 1 = Alta)
    bool tocado;
};

struct OrdenInfo {
    ulong ticket;
    double precioApertura;
    double stopLoss;
    double takeProfit;
    int tipo; // 0 = venta, 1 = compra
    bool breakEvenAplicado;
    int nivelBreakEven; // Nivel actual de break even (1 = inicial, 2 = primer ajuste, etc.)
    // Campos para detección de falsos positivos
    double distanciaMinima; // Distancia mínima alcanzada al precio de entrada
    bool marcadoParaEliminacion; // Si debe ser eliminada por falso positivo
    datetime tiempoColocacion; // Tiempo cuando se colocó la orden
};

#define IMBALANCE_BAJISTA 0
#define IMBALANCE_ALCISTA 1
#define OB_BAJISTA      0
#define OB_ALCISTA      1
#define LIQUIDEZ_BAJA   0
#define LIQUIDEZ_ALTA   1

// Variables globales
bool g_DebugMode = true;  // Variable para activar/desactivar los prints de debug
int g_TradesPorSesion = 0; // Contador de trades por sesión
string g_SesionActual = ""; // Sesión actual
double g_BalanceInicial = 0.0;  // Balance inicial de la cuenta
double g_UltimoRiesgoCalculado = 0.0; 
int g_ImbalanceContador = 0;
int g_OrderBlockContador = 0;  // Contador para IDs de OrderBlocks

// Controles de debug individuales por función
input group "Configuración de Debug (Solo para Backtesting)"
input bool     DebugConexion = false;        // Debug Conexión Broker
input bool     DebugSaldo = false;           // Debug Saldo Cuenta
input bool     DebugMargen = false;          // Debug Margen Disponible
input bool     DebugOtrasCondiciones = false; // Debug Otras Condiciones
input bool     DebugSesiones = false;        // Debug Configuración Sesiones
input bool     DebugRiesgo = false;          // Debug Configuración Riesgo
input bool     DebugDiaOperable = false;     // Debug Día Operable
input bool     DebugHorarioOperable = false; // Debug Horario Operable
input bool     DebugImbalances = false;      // Debug Imbalances
input bool     DebugOrderBlocks = false;     // Debug Order Blocks
input bool     DebugMaxMin = false;          // Debug Máximos y Mínimos
input bool     DebugLiquidez = false;        // Debug Liquidez Previa
input bool     DebugLotaje = false;          // Debug Cálculo de Lotaje
input bool     DebugBreakEven = false;       // Debug Break Even
input bool     DebugColocarOrden = false; 

// Arrays para almacenar estructuras
Imbalance g_Imbalances[];
OrderBlock g_OrderBlocks[];
MaximoMinimo g_MaximosMinimos[];
LiquidezPrevia g_LiquidezPrevia[];
OrdenInfo g_Ordenes[];
//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+
int OnInit()
{
    // Verificar condiciones iniciales
    if(!VerificarConexionBroker())
        return INIT_FAILED;
        
    if(!VerificarSaldoCuenta())
        return INIT_FAILED;
        
    if(!VerificarMargenDisponible())
        return INIT_FAILED;
        
    if(!VerificarOtrasCondiciones())
        return INIT_FAILED;
        
    if(!VerificarConfiguracionSesiones())
        return INIT_FAILED;
        
    if(!VerificarConfiguracionRiesgo())
        return INIT_FAILED;
    
    Print("SmartFlow 1 EA inicializado correctamente");
    return(INIT_SUCCEEDED);
}

//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{
    // Limpiar objetos gráficos y recursos
    Print("SmartFlow 1 EA finalizado. Razón: ", reason);
}

// Agregar esta función antes de OnTick()
bool TieneOperacionAbierta()
{
    for(int i = 0; i < PositionsTotal(); i++)
    {
        ulong ticket = PositionGetTicket(i);
        if(ticket > 0 && PositionGetString(POSITION_SYMBOL) == _Symbol)
        {
            return true;
        }
    }
    return false;
}

//+------------------------------------------------------------------+
//| Función para convertir hora del usuario a hora del servidor     |
//+------------------------------------------------------------------+
datetime ConvertirHoraUsuarioAServidor(datetime fechaUsuario)
{
    // Calcular diferencia horaria total
    int diferenciaHoras = ZonaHorariaUsuario;
    if(HorarioVeranoUsuario) diferenciaHoras += 1; // Agregar 1 hora si hay horario de verano
    
    // Convertir a hora del servidor (restar la diferencia porque vamos de usuario a servidor)
    datetime fechaServidor = fechaUsuario - (diferenciaHoras * 3600);
    
    return fechaServidor;
}

//+------------------------------------------------------------------+
//| Función para verificar restricciones por noticias fundamentales |
//+------------------------------------------------------------------+
bool VerificarRestriccionesNoticiasFundamentales()
{
    datetime tiempoActual = TimeCurrent();
    string simbolo = _Symbol;
    
    // Extraer las monedas del par
    string moneda1 = StringSubstr(simbolo, 0, 3);
    string moneda2 = StringSubstr(simbolo, 3, 3);
    
    // Margen de 30 minutos (1800 segundos)
    int margenSegundos = 30 * 60;
    
    // Verificar noticias USD si el par contiene USD
    if(moneda1 == "USD" || moneda2 == "USD")
    {
        // Noticias mensuales USD
        if(!USD_NominasNoAgricolas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_NominasNoAgricolas_Fecha)) <= margenSegundos) return false;
        if(!USD_IPC && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_IPC_Fecha)) <= margenSegundos) return false;
        if(!USD_PIB && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_PIB_Fecha)) <= margenSegundos) return false;
        if(!USD_VentasMinoristas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_VentasMinoristas_Fecha)) <= margenSegundos) return false;
        if(!USD_ProduccionIndustrial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_ProduccionIndustrial_Fecha)) <= margenSegundos) return false;
        
        // Noticias semanales USD
        if(!USD_SolicitudesDesempleo && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_SolicitudesDesempleo_Fecha)) <= margenSegundos) return false;
        if(!USD_InventariosPetroleo && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(USD_InventariosPetroleo_Fecha)) <= margenSegundos) return false;
    }
    
    // Verificar noticias EUR si el par contiene EUR
    if(moneda1 == "EUR" || moneda2 == "EUR")
    {
        // Noticias mensuales EUR
        if(!EUR_InflacionMensual && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(EUR_InflacionMensual_Fecha)) <= margenSegundos) return false;
        if(!EUR_PIB && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(EUR_PIB_Fecha)) <= margenSegundos) return false;
        if(!EUR_ProduccionIndustrial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(EUR_ProduccionIndustrial_Fecha)) <= margenSegundos) return false;
        if(!EUR_VentasMinoristas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(EUR_VentasMinoristas_Fecha)) <= margenSegundos) return false;
        if(!EUR_BalanzaComercial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(EUR_BalanzaComercial_Fecha)) <= margenSegundos) return false;
        
        // Noticias semanales EUR
        if(!EUR_ConfianzaConsumidor && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(EUR_ConfianzaConsumidor_Fecha)) <= margenSegundos) return false;
    }
    
    // Verificar noticias GBP si el par contiene GBP
    if(moneda1 == "GBP" || moneda2 == "GBP")
    {
        if(!GBP_InflacionMensual && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(GBP_InflacionMensual_Fecha)) <= margenSegundos) return false;
        if(!GBP_PIB && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(GBP_PIB_Fecha)) <= margenSegundos) return false;
        if(!GBP_VentasMinoristas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(GBP_VentasMinoristas_Fecha)) <= margenSegundos) return false;
        if(!GBP_ProduccionIndustrial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(GBP_ProduccionIndustrial_Fecha)) <= margenSegundos) return false;
        if(!GBP_BalanzaComercial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(GBP_BalanzaComercial_Fecha)) <= margenSegundos) return false;
    }
    
    // Verificar noticias JPY si el par contiene JPY
    if(moneda1 == "JPY" || moneda2 == "JPY")
    {
        if(!JPY_InflacionMensual && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(JPY_InflacionMensual_Fecha)) <= margenSegundos) return false;
        if(!JPY_PIB && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(JPY_PIB_Fecha)) <= margenSegundos) return false;
        if(!JPY_ProduccionIndustrial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(JPY_ProduccionIndustrial_Fecha)) <= margenSegundos) return false;
        if(!JPY_VentasMinoristas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(JPY_VentasMinoristas_Fecha)) <= margenSegundos) return false;
        if(!JPY_BalanzaComercial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(JPY_BalanzaComercial_Fecha)) <= margenSegundos) return false;
    }
    
    // Verificar noticias CAD si el par contiene CAD
    if(moneda1 == "CAD" || moneda2 == "CAD")
    {
        if(!CAD_InflacionMensual && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(CAD_InflacionMensual_Fecha)) <= margenSegundos) return false;
        if(!CAD_PIB && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(CAD_PIB_Fecha)) <= margenSegundos) return false;
        if(!CAD_VentasMinoristas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(CAD_VentasMinoristas_Fecha)) <= margenSegundos) return false;
        if(!CAD_BalanzaComercial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(CAD_BalanzaComercial_Fecha)) <= margenSegundos) return false;
    }
    
    // Verificar noticias AUD si el par contiene AUD
    if(moneda1 == "AUD" || moneda2 == "AUD")
    {
        if(!AUD_InflacionMensual && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(AUD_InflacionMensual_Fecha)) <= margenSegundos) return false;
        if(!AUD_PIB && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(AUD_PIB_Fecha)) <= margenSegundos) return false;
        if(!AUD_VentasMinoristas && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(AUD_VentasMinoristas_Fecha)) <= margenSegundos) return false;
        if(!AUD_BalanzaComercial && MathAbs(tiempoActual - ConvertirHoraUsuarioAServidor(AUD_BalanzaComercial_Fecha)) <= margenSegundos) return false;
    }
    
    // Verificar noticias CHF si el par contiene CHF
    if(moneda1 == "CHF" || moneda2 == "CHF")
    {
        if(!CHF_InflacionMensual && MathAbs(tiempoActual - CHF_InflacionMensual_Fecha) <= margenSegundos) return false;
        if(!CHF_PIB && MathAbs(tiempoActual - CHF_PIB_Fecha) <= margenSegundos) return false;
        if(!CHF_BalanzaComercial && MathAbs(tiempoActual - CHF_BalanzaComercial_Fecha) <= margenSegundos) return false;
    }
    
    return true; // No hay restricciones
}

//+------------------------------------------------------------------+
//| Función para cerrar trades por noticias fundamentales           |
//+------------------------------------------------------------------+
void CerrarTradesPorNoticiasFundamentales()
{
    if(!VerificarRestriccionesNoticiasFundamentales())
    {
        // Cerrar todas las posiciones abiertas
        for(int i = PositionsTotal() - 1; i >= 0; i--)
        {
            ulong ticket = PositionGetTicket(i);
            if(ticket > 0 && PositionGetString(POSITION_SYMBOL) == _Symbol)
            {
                if(!trade.PositionClose(ticket))
                {
                    Print("Error cerrando posición por noticia fundamental: ", GetLastError());
                }
                else
                {
                    Print("Posición cerrada por proximidad a noticia fundamental");
                }
            }
        }
        
        // Eliminar órdenes pendientes
        for(int i = OrdersTotal() - 1; i >= 0; i--)
        {
            ulong ticket = OrderGetTicket(i);
            if(ticket > 0 && OrderGetString(ORDER_SYMBOL) == _Symbol)
            {
                if(!trade.OrderDelete(ticket))
                {
                    Print("Error eliminando orden pendiente por noticia fundamental: ", GetLastError());
                }
                else
                {
                    Print("Orden pendiente eliminada por proximidad a noticia fundamental");
                }
            }
        }
    }
}

//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
{
    // Variables estáticas para optimización
    static datetime ultimaVelaOnTick = 0;
    static datetime ultimaVerificacionSesion = 0;
    
    // Verificar nueva vela solo cada 30 segundos para optimizar
    datetime tiempoActual = TimeCurrent();
    datetime velaActual = iTime(_Symbol, PERIOD_CURRENT, 0);
    
    // Siempre ejecutar funciones críticas
    CerrarOperacionesAntesCierreNY();
    CerrarTradesPorNoticiasFundamentales();
    
    // Detectar y eliminar falsos positivos en órdenes pendientes
    if(OrdersTotal() > 0)
    {
        DetectarFalsosPositivos();
    }
    
    // Solo aplicar break even si hay posiciones (optimización)
    if(PositionsTotal() > 0)
    {
        AplicarBreakEven();
        // Verificar órdenes de OrderBlocks ejecutadas
        VerificarOrdenesOrderBlocksEjecutadas();
        return; // No buscar nuevas oportunidades si ya hay posiciones
    }
    
    // Verificaciones de sesión solo cada minuto para optimizar
    if(tiempoActual - ultimaVerificacionSesion > 60)
    {
        ultimaVerificacionSesion = tiempoActual;
        
        // Verificar si es día operable
        if(!EsDiaOperable())
        {
            if(g_DebugMode) Print("El día de hoy no es un día operativo");
            return;
        }
        
        // Verificar si estamos en horario operable
        string sesion = "";
        if(!EsHorarioOperable(sesion))
        {
            if(g_DebugMode) Print("Mercado fuera de horario");
            return;
        }
        
        // Actualizar la sesión actual si ha cambiado
        if(g_SesionActual != sesion)
        {
            g_SesionActual = sesion;
            g_TradesPorSesion = 0; // Reiniciar contador al cambiar de sesión
        }
        
        // Verificar si ya se alcanzó el máximo de trades por sesión
        if(g_TradesPorSesion >= MaxTradesPorSesion)
        {
            if(g_DebugMode) Print("Máximo número de trades por sesión alcanzado");
            return;
        }
        
        // Verificar spread
        if(SymbolInfoInteger(_Symbol, SYMBOL_SPREAD) > MaxSpreadPermitido)
        {
            if(g_DebugMode) Print("Spread demasiado alto: ", SymbolInfoInteger(_Symbol, SYMBOL_SPREAD));
            return;
        }
        
        // Verificar restricciones por noticias fundamentales
        if(!VerificarRestriccionesNoticiasFundamentales())
        {
            if(g_DebugMode) Print("Operaciones restringidas por proximidad a noticia fundamental");
            return;
        }
    }
    
    // Solo ejecutar análisis en nueva vela para optimizar rendimiento
    if(velaActual == ultimaVelaOnTick) return;
    ultimaVelaOnTick = velaActual;
    
    // Verificar que al menos una función operativa esté activa
    if(!OperarImbalance && !OperarOrderblock && !OperarMaxMin && !OperarLiquidezPrevia)
    {
        if(g_DebugMode) Print("Advertencia: Ninguna función operativa está activa. Por favor, active al menos una.");
        return;
    }
    
    // Lógica de operación según configuración Smart Money (solo en nueva vela)
    if(OperarImbalance)
    {
        OperarImbalances();
    }
    
    if(OperarOrderblock)
    {
        OperarOrderBlocks();
    }
    
    if(OperarMaxMin)
    {
        OperarMaximosMinimos();
    }
    
    if(OperarLiquidezPrevia)
    {
        OperarLiquidezPrevia();
    }
}


//+------------------------------------------------------------------+
//| Funciones de verificación inicial                                |
//+------------------------------------------------------------------+
bool VerificarConexionBroker()
{
    if(g_DebugMode) Print("Verificando conexión con el broker...");
    
    if(!TerminalInfoInteger(TERMINAL_CONNECTED))
    {
        Print("No conexión con el servidor del Bróker");
        return false;
    }
    
    if(g_DebugMode) Print("Conexión con el broker establecida");
    return true;
}

bool VerificarSaldoCuenta()
{
    if(g_DebugMode) Print("Verificando saldo de la cuenta...");
    
    if(AccountInfoDouble(ACCOUNT_BALANCE) <= 0)
    {
        Print("Cuenta no tiene saldo disponible");
        return false;
    }
    
    if(g_DebugMode) Print("Saldo de cuenta verificado: ", AccountInfoDouble(ACCOUNT_BALANCE));
    return true;
}

bool VerificarMargenDisponible()
{
    if(g_DebugMode) Print("Verificando margen disponible...");
    
    if(AccountInfoDouble(ACCOUNT_MARGIN_FREE) <= 0)
    {
        Print("Cuenta no tiene margen disponible");
        return false;
    }
    
    if(g_DebugMode) Print("Margen disponible verificado: ", AccountInfoDouble(ACCOUNT_MARGIN_FREE));
    return true;
}

bool VerificarOtrasCondiciones()
{
    if(g_DebugMode) Print("Verificando otras condiciones...");
    
    // Verificar que el símbolo actual es válido
    if(_Symbol == "" || !SymbolInfoInteger(_Symbol, SYMBOL_SELECT))
    {
        Print("Símbolo no válido o no seleccionado");
        return false;
    }
    
    // Verificar que el trading está habilitado
    if(!MQLInfoInteger(MQL_TRADE_ALLOWED))
    {
        Print("Trading no permitido. Verifique configuración del terminal");
        return false;
    }
    
    if(g_DebugMode) Print("Otras condiciones verificadas correctamente");
    return true;
}

bool VerificarConfiguracionSesiones()
{
    if(g_DebugMode) Print("Verificando configuración de sesiones...");
    
    // Verificar que si EntradaPersonalizada está activa, las demás sesiones estén desactivadas
    if(EntradaPersonalizada && (OperarLondres || OperarNY || OperarTokyo))
    {
        Print("Entrada personalizada está activa y se requiere que desactives la operación por sesiones");
        return false;
    }
    
    if(g_DebugMode) Print("Configuración de sesiones verificada correctamente");
    return true;
}

bool VerificarConfiguracionRiesgo()
{
    if(g_DebugMode) Print("Verificando configuración de riesgo...");
    
    // Verificar que solo una opción de riesgo esté activa
    if(OperarRiesgoManual && OperarRiesgoautomático)
    {
        Print("Por favor definir si operar con riesgo automático o con riesgo manual");
        return false;
    }
    
    if(g_DebugMode) Print("Configuración de riesgo verificada correctamente");
    return true;
}

//+------------------------------------------------------------------+
//| Funciones de verificación de tiempo                              |
//+------------------------------------------------------------------+
bool EsDiaOperable()
{
    if(g_DebugMode) Print("Verificando si el día es operable...");
    
    MqlDateTime tiempo_actual;
    TimeToStruct(TimeCurrent(), tiempo_actual);
    
    bool es_operable = false;
    string dia = "";
    
    switch(tiempo_actual.day_of_week)
    {
        case 1: es_operable = Lunes; dia = "Lunes"; break;
        case 2: es_operable = Martes; dia = "Martes"; break;
        case 3: es_operable = Miercoles; dia = "Miércoles"; break;
        case 4: es_operable = Jueves; dia = "Jueves"; break;
        case 5: es_operable = Viernes; dia = "Viernes"; break;
        default: es_operable = false; dia = "Fin de semana";
    }
    
    if(es_operable && g_DebugMode)
        Print("El Mercado se encuentra en día operativo ", dia);
    
    return es_operable;
}

bool EsHorarioOperable(string &sesion)
{
    if(g_DebugMode) Print("Verificando si es horario operable...");
    
    MqlDateTime tiempo_actual;
    TimeToStruct(TimeCurrent(), tiempo_actual);
    
    int hora_actual = tiempo_actual.hour;
    
    // Ajustar por GMT Offset
    hora_actual = (hora_actual + GMTOffset) % 24;
    if(hora_actual < 0) hora_actual += 24;
    
    // Verificar si estamos en alguna sesión operable
    if(OperarLondres && hora_actual >= HoraInicioLondres && hora_actual < HoraCierreLondres)
    {
        sesion = "Londres";
        if(g_DebugMode) Print("Dentro de rango operativo, sesión: ", sesion);
        return true;
    }
    
    if(OperarNY && hora_actual >= HoraInicioNY && hora_actual < HoraCierreNY)
    {
        sesion = "Nueva York";
        if(g_DebugMode) Print("Dentro de rango operativo, sesión: ", sesion);
        return true;
    }
    
    if(OperarTokyo && hora_actual >= HoraInicioTokyo && hora_actual < HoraCierreTokyo)
    {
        sesion = "Tokyo";
        if(g_DebugMode) Print("Dentro de rango operativo, sesión: ", sesion);
        return true;
    }
    
    if(EntradaPersonalizada && hora_actual >= HoraInicioPersonal && hora_actual < HoraCierrePersonal)
    {
        sesion = "Personalizada";
        if(g_DebugMode) Print("Dentro de rango operativo, sesión: ", sesion);
        return true;
    }
    
    return false;
}


//+------------------------------------------------------------------+
//| Funciones de operación Smart Money                               |
//+------------------------------------------------------------------+
void OperarImbalances()
{
    bool debug = g_DebugMode && DebugImbalances;
    if(debug) Print("Analizando imbalances...");
    
    // Paso 1: Buscar imbalances
    if(debug) Print("Paso 1: Buscando imbalances...");
    BuscarImbalances();
    
    // Paso 3: Verificar conflictos con OrderBlocks si ambos están activos
    if(OperarOrderblock)
    {
        if(debug) Print("Paso 3: Verificando conflictos con OrderBlocks...");
        VerificarConflictosImbalanceOB();
    }
    
    // Paso 4: Graficar imbalances
    if(debug) Print("Paso 4: Graficando imbalances...");
    GraficarImbalances();
    
    // Paso 5: Colocar órdenes en imbalances
    if(debug) Print("Paso 5: Colocando órdenes en imbalances...");
    ColocarOrdenesImbalances();
    
    // Paso 6: Actualizar registro de órdenes
    if(debug) Print("Paso 6: Actualizando registro de órdenes...");
    ActualizarRegistroOrdenes();
}

//+------------------------------------------------------------------+
//| Función para buscar imbalances                                   |
//+------------------------------------------------------------------+
void BuscarImbalances()
{
    static datetime ultimaVela = 0;
    datetime tiempoActual = iTime(_Symbol, PERIOD_CURRENT, 0);
    if (tiempoActual == ultimaVela) return; // Solo actualizar si hay nueva vela
    ultimaVela = tiempoActual;
    
    bool debug = g_DebugMode && DebugImbalances;
    if(debug) Print("Iniciando búsqueda de imbalances...");
    
    // Limpiar array de imbalances previos
    ArrayFree(g_Imbalances);
    
    // Obtener datos de precios
    int maxVelas = MaxVelasAnalisis;
    if(maxVelas > Bars(_Symbol, PERIOD_CURRENT))
        maxVelas = Bars(_Symbol, PERIOD_CURRENT) - 10; // Asegurar que no exceda el número de velas disponibles
    
    if(debug) Print("Analizando ", maxVelas, " velas para buscar imbalances");
    
    // Buscar imbalances alcistas y bajistas
    // Empezamos desde i=1 para asegurarnos de que la vela i=0 (actual) esté cerrada
    for(int i = 1; i < maxVelas - 7; i++) // -7 para asegurar que podemos analizar hasta i+6
    {
        BuscarImbalanceAlcista(i);
        BuscarImbalanceBajista(i);
    }
    
    if(debug) Print("Búsqueda de imbalances completada. Total encontrados: ", ArraySize(g_Imbalances));
}

// Función para eliminar imbalances solapados antes de agregar uno nuevo
// Función para eliminar solo el imbalance más pequeño cuando hay solapamiento
bool EliminarImbalancesSolapados(Imbalance &nuevo_imbalance)
{
    bool debug = g_DebugMode && DebugImbalances;
    bool nuevo_es_menor = false;
    
    for(int i = ArraySize(g_Imbalances) - 1; i >= 0; i--)
    {
        if(g_Imbalances[i].tipo == nuevo_imbalance.tipo)
        {
            // Verificar si hay solapamiento
            bool solapa = false;
            double tolerancia = 5 * _Point;
            
            // Verificar solapamiento por tiempo (mismo tiempo de inicio)
            if(g_Imbalances[i].time == nuevo_imbalance.time)
            {
                solapa = true;
            }
            // Verificar solapamiento por rango de precios
            else
            {
                if(nuevo_imbalance.tipo == IMBALANCE_ALCISTA)
                {
                    // Caso: Mismo high pero diferentes low
                    if(MathAbs(nuevo_imbalance.high - g_Imbalances[i].high) <= tolerancia)
                    {
                        solapa = true;
                    }
                    // Caso: Mismo low pero diferentes high
                    else if(MathAbs(nuevo_imbalance.low - g_Imbalances[i].low) <= tolerancia)
                    {
                        solapa = true;
                    }
                    // Caso: Contención completa
                    else if((nuevo_imbalance.high >= g_Imbalances[i].high && nuevo_imbalance.low <= g_Imbalances[i].low) ||
                            (g_Imbalances[i].high >= nuevo_imbalance.high && g_Imbalances[i].low <= nuevo_imbalance.low))
                    {
                        solapa = true;
                    }
                }
                else // IMBALANCE_BAJISTA
                {
                    // Caso: Mismo high pero diferentes low
                    if(MathAbs(nuevo_imbalance.high - g_Imbalances[i].high) <= tolerancia)
                    {
                        solapa = true;
                    }
                    // Caso: Mismo low pero diferentes high
                    else if(MathAbs(nuevo_imbalance.low - g_Imbalances[i].low) <= tolerancia)
                    {
                        solapa = true;
                    }
                    // Caso: Contención completa
                    else if((nuevo_imbalance.high >= g_Imbalances[i].high && nuevo_imbalance.low <= g_Imbalances[i].low) ||
                            (g_Imbalances[i].high >= nuevo_imbalance.high && g_Imbalances[i].low <= nuevo_imbalance.low))
                    {
                        solapa = true;
                    }
                }
            }
            
            if(solapa)
            {
                // Calcular el tamaño de ambos imbalances
                double tamano_existente = MathAbs(g_Imbalances[i].high - g_Imbalances[i].low);
                double tamano_nuevo = MathAbs(nuevo_imbalance.high - nuevo_imbalance.low);
                
                if(debug)
                {
                    Print("Solapamiento detectado:");
                    Print("  Existente: High=", g_Imbalances[i].high, " Low=", g_Imbalances[i].low, " Tamaño=", tamano_existente);
                    Print("  Nuevo: High=", nuevo_imbalance.high, " Low=", nuevo_imbalance.low, " Tamaño=", tamano_nuevo);
                }
                
                // Si el nuevo imbalance es más pequeño, no lo agregamos
                if(tamano_nuevo <= tamano_existente)
                {
                    if(debug)
                    {
                        Print("El nuevo imbalance es más pequeño o igual. No se agrega.");
                    }
                    nuevo_es_menor = true;
                    break;
                }
                // Si el nuevo imbalance es más grande, eliminamos el existente
                else
                {
                    if(debug)
                    {
                        Print("Eliminando imbalance existente más pequeño: ID=", g_Imbalances[i].id);
                    }
                    
                    // Eliminar el imbalance existente más pequeño
                     for(int j = i; j < ArraySize(g_Imbalances) - 1; j++)
                     {
                         g_Imbalances[j] = g_Imbalances[j + 1];
                     }
                     ArrayResize(g_Imbalances, ArraySize(g_Imbalances) - 1);
                 }
             }
         }
     }
     
     return nuevo_es_menor; // Retorna true si el nuevo imbalance es menor y no debe agregarse
 }

void BuscarImbalanceAlcista(int i)
{
    bool debug = g_DebugMode && DebugImbalances;
    
    double low_i = iLow(_Symbol, PERIOD_CURRENT, i);
    double high_i2 = iHigh(_Symbol, PERIOD_CURRENT, i+2);
    
    if(low_i < high_i2)
        return; // No hay imbalance alcista
    if((low_i - high_i2) < 20 * _Point)
    return; // Imbalance demasiado pequeño, ignorar
    
    Imbalance imb;
    imb.tipo = IMBALANCE_ALCISTA;
    imb.high = low_i;
    imb.low = high_i2;
    imb.tamano = low_i - high_i2;
    imb.time = iTime(_Symbol, PERIOD_CURRENT, i+2);
    imb.unificado = false;
    imb.operado = false;
    imb.vela_inicio = i+2;
    imb.vela_fin = i;

    bool continuar = true;
    int vela_actual = 3;

    while(continuar && (i+vela_actual) < Bars(_Symbol, PERIOD_CURRENT))
    {
        double low_prev = iLow(_Symbol, PERIOD_CURRENT, i+vela_actual-2);
        double high_prev = iHigh(_Symbol, PERIOD_CURRENT, i+vela_actual-1);
        double high_current = iHigh(_Symbol, PERIOD_CURRENT, i+vela_actual);

        if(low_prev > high_current && (high_prev - low_prev) < DistanciaUnirImbalance * _Point)
        {
            imb.low = high_current;
            imb.tamano = imb.high - imb.low;
            imb.time = iTime(_Symbol, PERIOD_CURRENT, i+vela_actual);
            imb.unificado = true;
            imb.vela_inicio = i+vela_actual;

            if(debug) Print("Imbalance alcista unificado hasta vela i+", vela_actual);

            vela_actual++;
            if(vela_actual > 6)
                break;
        }
        else
        {
            continuar = false;
        }
    }

    if(imb.tamano >= TamanoMinImbalance * _Point)
    {
        // Calcular punto medio
        imb.punto_medio = imb.low + (imb.tamano / 2);

        // Verificar mitigación desde la vela i hacia atrás
        imb.mitigado = false;

        int velas_a_revisar = 500;
        for(int j = 1; j <= velas_a_revisar && (i-j) >= 0; j++)
        {
            double low_check = iLow(_Symbol, PERIOD_CURRENT, i-j);

            if(low_check <= imb.punto_medio)
            {
                imb.mitigado = true;
                break;
            }
        }

        // Establecer si es operable
        imb.operable = !imb.mitigado;

        // Asignar ID
        g_ImbalanceContador++;
        imb.id = "IB_" + IntegerToString(g_ImbalanceContador) + "_" + TimeToString(iTime(_Symbol, PERIOD_CURRENT, imb.vela_inicio), TIME_DATE|TIME_MINUTES);

        // NUEVO: Verificar si el imbalance debe agregarse (eliminar solapados)
        if(!EliminarImbalancesSolapados(imb))
        {
            // Guardar imbalance solo si no es menor que uno existente
            int size = ArraySize(g_Imbalances);
            ArrayResize(g_Imbalances, size + 1);
            g_Imbalances[size] = imb;
        }

        if(debug)
        {
            Print("Imbalance alcista guardado - ID: ", imb.id,
                  " | High: ", imb.high,
                  " | Low: ", imb.low,
                  " | Tamaño: ", imb.tamano / _Point, " pts",
                  " | Mitigado: ", imb.mitigado,
                  " | Operable: ", imb.operable);
        }
    }
    else if(debug)
    {
        Print("Imbalance alcista descartado por tamaño insuficiente. Tamaño: ", imb.tamano / _Point, " puntos, Mínimo requerido: ", TamanoMinImbalance);
    }
}

void BuscarImbalanceBajista(int i)
{
    bool debug = g_DebugMode && DebugImbalances;
    
    double high_i = iHigh(_Symbol, PERIOD_CURRENT, i);
    double low_i2 = iLow(_Symbol, PERIOD_CURRENT, i+2);
    
    if(high_i >= low_i2)
        return; // No hay imbalance bajista
    if((low_i2 - high_i) < 20 * _Point)
    return; // Imbalance demasiado pequeño, ignorar
    
    Imbalance imb;
    imb.tipo = IMBALANCE_BAJISTA;
    imb.high = low_i2;
    imb.low = high_i;
    imb.tamano = low_i2 - high_i;
    imb.time = iTime(_Symbol, PERIOD_CURRENT, i+2);
    imb.unificado = false;
    imb.mitigado = false;
    imb.operado = false;
    imb.vela_inicio = i+2;
    imb.vela_fin = i;

    bool continuar = true;
    int vela_actual = 3;

    while(continuar && (i+vela_actual) < Bars(_Symbol, PERIOD_CURRENT))
    {
        double high_prev = iHigh(_Symbol, PERIOD_CURRENT, i+vela_actual-2);
        double low_prev = iLow(_Symbol, PERIOD_CURRENT, i+vela_actual-1);
        double low_current = iLow(_Symbol, PERIOD_CURRENT, i+vela_actual);

        if(high_prev < low_current && (low_prev - high_prev) < DistanciaUnirImbalance * _Point)
        {
            imb.high = low_current;
            imb.tamano = imb.high - imb.low;
            imb.time = iTime(_Symbol, PERIOD_CURRENT, i+vela_actual);
            imb.unificado = true;
            imb.vela_inicio = i+vela_actual;

            if(debug) Print("Imbalance bajista unificado hasta vela i+", vela_actual);

            vela_actual++;
            if(vela_actual > 6)
                break;
        }
        else
        {
            continuar = false;
        }
    }

    if(imb.tamano >= TamanoMinImbalance * _Point)
    {
        // Calcular punto medio del imbalance bajista
        imb.punto_medio = imb.high - (imb.tamano / 2);

        // Verificar si fue mitigado
        imb.mitigado = false;
        int velas_a_revisar = 500;

        for(int j = 1; j <= velas_a_revisar && (i-j) >= 0; j++)
        {
            double high_check = iHigh(_Symbol, PERIOD_CURRENT, i-j);

            if(high_check >= imb.punto_medio)
            {
                imb.mitigado = true;
                break;
            }
        }

        // Establecer si es operable
        imb.operable = !imb.mitigado;

        // Asignar ID
        g_ImbalanceContador++;
        imb.id = "IB_" + IntegerToString(g_ImbalanceContador) + "_" + TimeToString(iTime(_Symbol, PERIOD_CURRENT, imb.vela_inicio), TIME_DATE|TIME_MINUTES);

        // NUEVO: Verificar si el imbalance debe agregarse (eliminar solapados)
        if(!EliminarImbalancesSolapados(imb))
        {
            // Guardar imbalance solo si no es menor que uno existente
            int size = ArraySize(g_Imbalances);
            ArrayResize(g_Imbalances, size + 1);
            g_Imbalances[size] = imb;
        }

        if(debug)
        {
            Print("Imbalance bajista guardado - ID: ", imb.id,
                  " | High: ", imb.high,
                  " | Low: ", imb.low,
                  " | Tamaño: ", imb.tamano / _Point, " pts",
                  " | Mitigado: ", imb.mitigado,
                  " | Operable: ", imb.operable);
        }
    }
    else if(debug)
    {
    Print("Imbalance bajista descartado por tamaño insuficiente. Tamaño: ", imb.tamano / _Point, " puntos, Mínimo requerido: ", TamanoMinImbalance);
    }
}

void GraficarImbalances()
{
    int total = ArraySize(g_Imbalances);
    for(int i = 0; i < total; i++)
    {
        Imbalance imb = g_Imbalances[i];
        string etiqueta = imb.id;
        string nombreObjeto = "IB_" + IntegerToString(i);

        // Eliminar objeto si ya existe
        ObjectDelete(0, nombreObjeto);

        datetime time_inicio = iTime(_Symbol, PERIOD_CURRENT, imb.vela_inicio);
        datetime time_fin    = iTime(_Symbol, PERIOD_CURRENT, imb.vela_fin);
        datetime time_ext    = TimeCurrent(); // Para operables se extiende hasta ahora

        // Variables para coordenadas del rectángulo
        double price_top, price_bottom;
        color color_fill = clrNONE;
        color color_border;
        ENUM_LINE_STYLE estilo = STYLE_SOLID;

        if(imb.mitigado && !imb.operable)
        {
            if(imb.tipo == IMBALANCE_ALCISTA)
            {
                price_top = iHigh(_Symbol, PERIOD_CURRENT, imb.vela_inicio);
                price_bottom = iLow(_Symbol, PERIOD_CURRENT, imb.vela_fin);
                color_fill = ColorImbalanceAlcistaMitigado;
            }
            else
            {
                price_top = iLow(_Symbol, PERIOD_CURRENT, imb.vela_inicio);
                price_bottom = iHigh(_Symbol, PERIOD_CURRENT, imb.vela_fin);
                color_fill = ColorImbalanceBajistaMitigado;
            }
            color_border = color_fill;
        }
        else if(!imb.mitigado && imb.operable)
        {
            if(imb.tipo == IMBALANCE_ALCISTA)
            {
                price_top = iHigh(_Symbol, PERIOD_CURRENT, imb.vela_inicio);
                price_bottom = iLow(_Symbol, PERIOD_CURRENT, imb.vela_fin);
                color_border = ColorImbalanceAlcista;
            }
            else
            {
                price_top = iLow(_Symbol, PERIOD_CURRENT, imb.vela_inicio);
                price_bottom = iHigh(_Symbol, PERIOD_CURRENT, imb.vela_fin);
                color_border = ColorImbalanceBajista;
            }
            estilo = STYLE_DASH;
            time_fin = time_ext;
        }
        else
        {
            continue; // Ni mitigado ni operable, no graficar
        }

        // Dibujar rectángulo
        if(!ObjectCreate(0, nombreObjeto, OBJ_RECTANGLE, 0, time_inicio, price_top, time_fin, price_bottom))
        {
            Print("Error creando rectángulo: ", GetLastError());
            continue;
        }

        ObjectSetInteger(0, nombreObjeto, OBJPROP_COLOR, color_border);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_STYLE, estilo);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_WIDTH, 1);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_BACK, true);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_RAY_RIGHT, false);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_SELECTABLE, true);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_SELECTED, false);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_HIDDEN, false);

        // Relleno solo para mitigados
        ObjectSetInteger(0, nombreObjeto, OBJPROP_FILL, imb.mitigado && !imb.operable);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_WIDTH, 1);
        ObjectSetInteger(0, nombreObjeto, OBJPROP_CORNER, CORNER_LEFT_UPPER);

        // Tooltip
        ObjectSetString(0, nombreObjeto, OBJPROP_TEXT, etiqueta);

        // Debug
        if(g_DebugMode)
        {
            Print("Graficado imbalance: ", etiqueta,
                  " | Mitigado: ", imb.mitigado,
                  " | Operable: ", imb.operable,
                  " | High: ", imb.high,
                  " | Low: ", imb.low,
                  " | Punto medio: ", imb.punto_medio);
        }
    }
}


//+------------------------------------------------------------------+
//| Función para colocar órdenes en imbalances operables             |
//+------------------------------------------------------------------+
// Verifica si ya existe una orden pendiente con ese comentario
bool YaExisteOrdenPendiente(string comentario)
{
    int total = OrdersTotal();
    for(int i = 0; i < total; i++)
    {
        ulong ticket = OrderGetTicket(i);
        if(OrderSelect(ticket))
        {
            long tipo = OrderGetInteger(ORDER_TYPE);
            string comentarioOrden = OrderGetString(ORDER_COMMENT);

            if((tipo == ORDER_TYPE_BUY_LIMIT || tipo == ORDER_TYPE_SELL_LIMIT) &&
               comentarioOrden == comentario)
            {
                return true;
            }
        }
    }
    return false;
}

// Verifica si ya hay una orden pendiente cerca del punto medio
bool ExisteOrdenCercana(double puntoMedio, double distanciaMinima)
{
    int total = OrdersTotal();
    for(int i = 0; i < total; i++)
    {
        ulong ticket = OrderGetTicket(i);
        if(OrderSelect(ticket))
        {
            long tipo = OrderGetInteger(ORDER_TYPE);
            if(tipo == ORDER_TYPE_BUY_LIMIT || tipo == ORDER_TYPE_SELL_LIMIT)
            {
                double precioOrden = OrderGetDouble(ORDER_PRICE_OPEN);
                if(MathAbs(precioOrden - puntoMedio) <= distanciaMinima * _Point)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// Coloca órdenes en los imbalances operables y no mitigados
void ColocarOrdenesImbalances()
{
    int totalImbalances = ArraySize(g_Imbalances);
    double precioActual = SymbolInfoDouble(_Symbol, SYMBOL_BID); // o SYMBOL_ASK

    for(int i = 0; i < totalImbalances; i++)
    {
        if(!g_Imbalances[i].operable)
        {
            Print("Imbalance ", g_Imbalances[i].id, " no es operable.");
            continue;
        }

        if(g_Imbalances[i].mitigado)
        {
            Print("Imbalance ", g_Imbalances[i].id, " ya fue mitigado.");
            continue;
        }
        if(g_Imbalances[i].operado)
        {
            Print("Imbalance ", g_Imbalances[i].id, " ya fue operado.");
            continue;
        }

        double puntoMedio = g_Imbalances[i].punto_medio;
        ENUM_ORDER_TYPE tipoOrden = (g_Imbalances[i].tipo == IMBALANCE_ALCISTA) ? ORDER_TYPE_BUY_LIMIT : ORDER_TYPE_SELL_LIMIT;
        string comentario = "Imbalance " + g_Imbalances[i].id;

        // Condición (a): distancia del precio al punto medio
        if(MathAbs(precioActual - puntoMedio) > DistanciaMaximaAlPuntoEntrada * _Point)
        {
            Print("Imbalance ", g_Imbalances[i].id, " descartado: el precio está a más de ", DistanciaMaximaAlPuntoEntrada, " puntos.");
            continue;
        }

        // Condición (b): verificar orden cercana
        if(ExisteOrdenCercana(puntoMedio, DistanciaMinimaEntreOrdenes))
        {
            Print("Imbalance ", g_Imbalances[i].id, " descartado: ya hay una orden pendiente cerca (menos de ", DistanciaMinimaEntreOrdenes, " puntos).");
            continue;
        }

        // Verificar duplicado por comentario
        if(YaExisteOrdenPendiente(comentario))
        {
            Print("Ya existe una orden pendiente para el imbalance ", g_Imbalances[i].id);
            continue;
        }

        // Colocar la orden
        if(ColocarOrden(tipoOrden, puntoMedio, comentario))
        {
            g_Imbalances[i].mitigado = true;
            g_Imbalances[i].operado = true;
            Print("Orden colocada en imbalance ", g_Imbalances[i].id, ". Marcado como mitigado.");
        }
        else
        {
            Print("Error al colocar orden en imbalance ", g_Imbalances[i].id);
        }
    }
}


//+------------------------------------------------------------------+
//| Función para actualizar registro de órdenes                      |
//+------------------------------------------------------------------+
void ActualizarRegistroOrdenes()
{
    bool debug = g_DebugMode && DebugImbalances;
    if(debug) Print("Actualizando registro de órdenes...");
    
    // Verificar órdenes pendientes
    int totalOrdenes = OrdersTotal();
    
    for(int i = 0; i < totalOrdenes; i++)
    {
        ulong ticket = OrderGetTicket(i);
        if(ticket <= 0) continue;
        
        // Verificar si la orden pertenece al símbolo actual
        if(OrderGetString(ORDER_SYMBOL) != _Symbol) continue;
        
        // Verificar si la sesión ha cerrado
        string sesionActual = "";
        if(!EsHorarioOperable(sesionActual) && sesionActual != g_SesionActual)
        {
            // Cancelar órdenes pendientes si la sesión ha cerrado
            trade.OrderDelete(ticket);
            if(debug) Print("Orden #", ticket, " cancelada por cierre de sesión");
        }
    }
    
    // Verificar órdenes de OrderBlocks ejecutadas
    VerificarOrdenesOrderBlocksEjecutadas();
    
    // Aplicar break even a posiciones abiertas
    AplicarBreakEven();
    
    if(debug) Print("Actualización de registro de órdenes completada");
}

//+------------------------------------------------------------------+
//| Función para verificar si órdenes de OrderBlocks se ejecutaron  |
//+------------------------------------------------------------------+
void VerificarOrdenesOrderBlocksEjecutadas()
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Verificando órdenes de OrderBlocks ejecutadas...");
    
    // Revisar todas las posiciones abiertas
    for(int i = 0; i < PositionsTotal(); i++)
    {
        ulong ticket = PositionGetTicket(i);
        if(ticket <= 0) continue;
        
        // Verificar si la posición pertenece al símbolo actual
        if(PositionGetString(POSITION_SYMBOL) != _Symbol) continue;
        
        // Obtener el comentario de la posición
        string comentario = PositionGetString(POSITION_COMMENT);
        
        // Verificar si es una orden de OrderBlock
        if(StringFind(comentario, "OrderBlock ") == 0)
        {
            // Extraer el ID del OrderBlock del comentario
            string partes[];
            int numPartes = StringSplit(comentario, '_', partes);
            
            if(numPartes >= 1)
            {
                string idOrderBlock = StringSubstr(partes[0], 11); // Remover "OrderBlock "
                
                // Buscar el OrderBlock correspondiente
                for(int j = 0; j < ArraySize(g_OrderBlocks); j++)
                {
                    if(g_OrderBlocks[j].id == idOrderBlock)
                    {
                        // Marcar como operado y mitigado si aún no lo está
                        if(!g_OrderBlocks[j].operado || !g_OrderBlocks[j].mitigado)
                        {
                            g_OrderBlocks[j].operado = true;
                            g_OrderBlocks[j].mitigado = true;
                            
                            if(debug) Print("OrderBlock ", idOrderBlock, " marcado como operado y mitigado debido a ejecución de orden #", ticket);
                        }
                        break;
                    }
                }
            }
        }
    }
}

  

void OperarOrderBlocks()
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Analizando order blocks...");
    
    // Paso 1: Buscar imbalances (prerequisito para OrderBlocks)
    if(debug) Print("Paso 1: Buscando imbalances para OrderBlocks...");
    BuscarImbalances();
    
    // Paso 2: Buscar OrderBlocks basados en imbalances
    if(debug) Print("Paso 2: Buscando OrderBlocks...");
    BuscarOrderBlocks();
    
    // Paso 3: Verificar conflictos con Imbalances si ambos están activos
    if(OperarImbalance)
    {
        if(debug) Print("Paso 3: Verificando conflictos con Imbalances...");
        VerificarConflictosImbalanceOB();
    }
    
    // Paso 4: Verificar conflictos con MaxMin si ambos están activos
    if(OperarMaxMin)
    {
        if(debug) Print("Paso 4: Verificando conflictos con MaxMin...");
        VerificarConflictosOBMaxMinNuevo();
    }
    
    // Paso 5: Aplicar confirmaciones
    if(debug) Print("Paso 5: Aplicando confirmaciones...");
    AplicarConfirmacionesOB();
    
    // Paso 6: Graficar OrderBlocks
    if(debug) Print("Paso 6: Graficando OrderBlocks...");
    GraficarOrderBlocks();
    
    // Paso 7: Colocar órdenes en OrderBlocks
    if(debug) Print("Paso 7: Colocando órdenes en OrderBlocks...");
    ColocarOrdenesOrderBlocks();
}

//+------------------------------------------------------------------+
//| Función para buscar OrderBlocks basados en imbalances           |
//+------------------------------------------------------------------+
void BuscarOrderBlocks()
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Iniciando búsqueda de OrderBlocks...");
    
    // Limpiar array de OrderBlocks
    ArrayResize(g_OrderBlocks, 0);
    
    // Iterar sobre todos los imbalances encontrados
    for(int i = 0; i < ArraySize(g_Imbalances); i++)
    {
        if(debug) Print("Evaluando imbalance ", i, " para OrderBlock");
        
        // Validar si el imbalance es suficientemente grande para OB
        if(ValidarImbalanceParaOB(g_Imbalances[i]))
        {
            if(debug) Print("Imbalance ", i, " válido para OB. Tipo: ", g_Imbalances[i].tipo);
            
            // Buscar OrderBlock según el tipo de imbalance
            if(g_Imbalances[i].tipo == 1) // Alcista
            {
                BuscarOrderBlockAlcista(g_Imbalances[i]);
            }
            else if(g_Imbalances[i].tipo == 0) // Bajista
            {
                BuscarOrderBlockBajista(g_Imbalances[i]);
            }
        }
        else
        {
            if(debug) Print("Imbalance ", i, " no válido para OB. Tamaño: ", g_Imbalances[i].tamano);
        }
    }
    
    if(debug) Print("Búsqueda de OrderBlocks completada. Total encontrados: ", ArraySize(g_OrderBlocks));
}

//+------------------------------------------------------------------+
//| Función para validar si un imbalance es válido para OB          |
//+------------------------------------------------------------------+
bool ValidarImbalanceParaOB(Imbalance &imb)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    
    // Verificar si el tamaño del imbalance es mayor o igual al mínimo requerido
    bool es_valido = (imb.tamano >= TamanoIBMinparaOB * _Point);
    
    if(debug)
    {
        Print("Validando imbalance para OB:");
        Print("  - Tamaño imbalance: ", imb.tamano);
        Print("  - Tamaño mínimo requerido: ", TamanoIBMinparaOB * _Point);
        Print("  - Es válido: ", es_valido);
    }
    
    return es_valido;
}

//+------------------------------------------------------------------+
//| Función para buscar OrderBlock Alcista                          |
//+------------------------------------------------------------------+
void BuscarOrderBlockAlcista(Imbalance &imb)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Buscando OrderBlock Alcista para imbalance en vela ", imb.vela_inicio);
    
    // Buscar la primera vela bajista desde la vela del imbalance (i) hasta i+5
    for(int j = 0; j <= 5; j++)
    {
        int vela_ob = imb.vela_inicio + j;
        
        // Verificar que no nos salgamos del rango de datos
        if(vela_ob >= Bars(_Symbol, PERIOD_CURRENT))
        {
            if(debug) Print("Vela ", vela_ob, " fuera del rango de datos");
            break;
        }
        
        // Obtener datos de la vela
        double open = iOpen(_Symbol, PERIOD_CURRENT, vela_ob);
        double close = iClose(_Symbol, PERIOD_CURRENT, vela_ob);
        double high = iHigh(_Symbol, PERIOD_CURRENT, vela_ob);
        double low = iLow(_Symbol, PERIOD_CURRENT, vela_ob);
        datetime time = iTime(_Symbol, PERIOD_CURRENT, vela_ob);
        
        // Verificar si es vela bajista (close < open)
        if(close < open)
        {
            if(debug) Print("Vela bajista encontrada en posición ", vela_ob, " (i+", j, ")");
            
            // Crear OrderBlock
            OrderBlock nuevo_ob;
            g_OrderBlockContador++;
            nuevo_ob.id = "OB_" + TimeToString(time, TIME_DATE|TIME_MINUTES) + "_" + IntegerToString(g_OrderBlockContador);
            nuevo_ob.time = time;
            nuevo_ob.high = high;
            nuevo_ob.low = low;
            nuevo_ob.tipo = 1; // Alcista
            nuevo_ob.tamano = high - low;
            nuevo_ob.punto_medio = low + (nuevo_ob.tamano / 2.0);
            nuevo_ob.vela_inicio = vela_ob;
            nuevo_ob.operado = false;
            
            // Verificar mitigación desde una vela después del imbalance hacia el presente
            nuevo_ob.mitigado = false;
            int velas_a_revisar = 500;
            for(int k = 0; k < velas_a_revisar && (imb.vela_inicio-3-k) >= 0; k++)
            {
                double low_check = iLow(_Symbol, PERIOD_CURRENT, imb.vela_inicio-3-k);
                
                // Para OB Alcista: mitigado si el precio alcanza el 50% del OB (punto medio)
                if(low_check <= nuevo_ob.punto_medio)
                {
                    nuevo_ob.mitigado = true;
                    if(debug) Print("OB Alcista ", nuevo_ob.id, " marcado como mitigado. Precio: ", low_check, " <= Punto medio: ", nuevo_ob.punto_medio);
                    break;
                }
            }
            
            // Establecer si es operable (no mitigado y no operado)
            nuevo_ob.operable = !nuevo_ob.mitigado && !nuevo_ob.operado;
            
            // Inicializar confirmaciones
            nuevo_ob.confluencia = false;
            nuevo_ob.breakofstructure = false;
            nuevo_ob.liquidez_previa = false;
            nuevo_ob.induccion = false;
            nuevo_ob.patron_mw = false;
            
            // Agregar al array
            int size = ArraySize(g_OrderBlocks);
            ArrayResize(g_OrderBlocks, size + 1);
            g_OrderBlocks[size] = nuevo_ob;
            
            if(debug)
            {
                Print("OrderBlock Alcista creado:");
                Print("  - ID: ", nuevo_ob.id);
                Print("  - High: ", nuevo_ob.high);
                Print("  - Low: ", nuevo_ob.low);
                Print("  - Tamaño: ", nuevo_ob.tamano);
                Print("  - Punto medio: ", nuevo_ob.punto_medio);
            }
            
            return; // Salir después de encontrar la primera vela bajista
        }
    }
    
    if(debug) Print("No se encontró OrderBlock Alcista para el imbalance");
}

//+------------------------------------------------------------------+
//| Función para buscar OrderBlock Bajista                          |
//+------------------------------------------------------------------+
void BuscarOrderBlockBajista(Imbalance &imb)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Buscando OrderBlock Bajista para imbalance en vela ", imb.vela_inicio);
    
    // Buscar la primera vela alcista desde la vela del imbalance (i) hasta i+5
    for(int j = 0; j <= 5; j++)
    {
        int vela_ob = imb.vela_inicio + j;
        
        // Verificar que no nos salgamos del rango de datos
        if(vela_ob >= Bars(_Symbol, PERIOD_CURRENT))
        {
            if(debug) Print("Vela ", vela_ob, " fuera del rango de datos");
            break;
        }
        
        // Obtener datos de la vela
        double open = iOpen(_Symbol, PERIOD_CURRENT, vela_ob);
        double close = iClose(_Symbol, PERIOD_CURRENT, vela_ob);
        double high = iHigh(_Symbol, PERIOD_CURRENT, vela_ob);
        double low = iLow(_Symbol, PERIOD_CURRENT, vela_ob);
        datetime time = iTime(_Symbol, PERIOD_CURRENT, vela_ob);
        
        // Verificar si es vela alcista (close > open)
        if(close > open)
        {
            if(debug) Print("Vela alcista encontrada en posición ", vela_ob, " (i+", j, ")");
            
            // Crear OrderBlock
            OrderBlock nuevo_ob;
            g_OrderBlockContador++;
            nuevo_ob.id = "OB_" + TimeToString(time, TIME_DATE|TIME_MINUTES) + "_" + IntegerToString(g_OrderBlockContador);
            nuevo_ob.time = time;
            nuevo_ob.high = high;
            nuevo_ob.low = low;
            nuevo_ob.tipo = 0; // Bajista
            nuevo_ob.tamano = high - low;
            nuevo_ob.punto_medio = low + (nuevo_ob.tamano / 2.0);
            nuevo_ob.vela_inicio = vela_ob;
            nuevo_ob.operado = false;
            
            // Verificar mitigación desde una vela después del imbalance hacia el presente
            nuevo_ob.mitigado = false;
            int velas_a_revisar = 500;
            for(int k = 0; k < velas_a_revisar && (imb.vela_inicio-3-k) >= 0; k++)
            {
                double high_check = iHigh(_Symbol, PERIOD_CURRENT, imb.vela_inicio-3-k);
                
                // Para OB Bajista: mitigado si el precio alcanza el 50% del OB (punto medio)
                if(high_check >= nuevo_ob.punto_medio)
                {
                    nuevo_ob.mitigado = true;
                    if(debug) Print("OB Bajista ", nuevo_ob.id, " marcado como mitigado. Precio: ", high_check, " >= Punto medio: ", nuevo_ob.punto_medio);
                    break;
                }
            }
            
            // Establecer si es operable (no mitigado y no operado)
            nuevo_ob.operable = !nuevo_ob.mitigado && !nuevo_ob.operado;
            
            // Inicializar confirmaciones
            nuevo_ob.confluencia = false;
            nuevo_ob.breakofstructure = false;
            nuevo_ob.liquidez_previa = false;
            nuevo_ob.induccion = false;
            nuevo_ob.patron_mw = false;
            
            // Agregar al array
            int size = ArraySize(g_OrderBlocks);
            ArrayResize(g_OrderBlocks, size + 1);
            g_OrderBlocks[size] = nuevo_ob;
            
            if(debug)
            {
                Print("OrderBlock Bajista creado:");
                Print("  - ID: ", nuevo_ob.id);
                Print("  - High: ", nuevo_ob.high);
                Print("  - Low: ", nuevo_ob.low);
                Print("  - Tamaño: ", nuevo_ob.tamano);
                Print("  - Punto medio: ", nuevo_ob.punto_medio);
            }
            
            return; // Salir después de encontrar la primera vela alcista
        }
    }
    
    if(debug) Print("No se encontró OrderBlock Bajista para el imbalance");
}

void OperarMaximosMinimos()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Analizando máximos y mínimos...");
    
    // Paso 1: Buscar máximos y mínimos
    if(debug) Print("Paso 1: Buscando máximos y mínimos...");
    BuscarMaximosMinimos();
    
    // Paso 2: Graficar máximos y mínimos
    if(debug) Print("Paso 2: Graficando máximos y mínimos...");
    GraficarMaximosMinimos();
    
    // Paso 3: Evaluar convergencias y operar
    if(debug) Print("Paso 3: Evaluando convergencias y operando...");
    EvaluarConvergenciasMaxMin();
    
    // Nota: Los conflictos con OrderBlocks se manejan en OperarOrderBlocks()
    // para evitar duplicación y mantener el orden de prioridades correcto
}

//+------------------------------------------------------------------+
//| Función para buscar máximos y mínimos                           |
//+------------------------------------------------------------------+
void BuscarMaximosMinimos()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Iniciando búsqueda de máximos y mínimos...");
    
    // Limpiar array previo
    ArrayResize(g_MaximosMinimos, 0);
    
    // Buscar máximos y mínimos diarios
    BuscarMaxMinDiario();
    
    // Buscar máximos y mínimos semanales
    BuscarMaxMinSemanal();
    
    // Buscar máximos y mínimos mensuales
    BuscarMaxMinMensual();
    
    // Buscar máximos y mínimos anuales
    BuscarMaxMinAnual();
    
    if(debug) Print("Búsqueda de máximos y mínimos completada. Total encontrados: ", ArraySize(g_MaximosMinimos));
}

//+------------------------------------------------------------------+
//| Función para buscar máximo y mínimo diario                      |
//+------------------------------------------------------------------+
void BuscarMaxMinDiario()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Buscando máximo y mínimo del día anterior...");
    
    MqlDateTime dt;
    TimeToStruct(TimeCurrent(), dt);
    
    // Calcular inicio y fin del día anterior
    datetime inicio_dia_anterior = StringToTime(StringFormat("%04d.%02d.%02d 00:00", dt.year, dt.mon, dt.day)) - 24 * 3600;
    datetime fin_dia_anterior = StringToTime(StringFormat("%04d.%02d.%02d 23:59", dt.year, dt.mon, dt.day)) - 24 * 3600;
    
    double max_diario = 0;
    double min_diario = DBL_MAX;
    datetime time_max = 0;
    datetime time_min = 0;
    
    // Buscar en las velas del día anterior
    for(int i = 0; i < Bars(_Symbol, PERIOD_CURRENT); i++)
    {
        datetime vela_time = iTime(_Symbol, PERIOD_CURRENT, i);
        
        // Si la vela es anterior al día anterior, parar
        if(vela_time < inicio_dia_anterior) break;
        
        // Si la vela es posterior al día anterior, continuar
        if(vela_time > fin_dia_anterior) continue;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, i);
        double low = iLow(_Symbol, PERIOD_CURRENT, i);
        
        if(high > max_diario)
        {
            max_diario = high;
            time_max = vela_time;
        }
        
        if(low < min_diario)
        {
            min_diario = low;
            time_min = vela_time;
        }
    }
    
    // Agregar máximo diario
    if(max_diario > 0)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_max;
        g_MaximosMinimos[size].maximo = max_diario;
        g_MaximosMinimos[size].minimo = 0;
        g_MaximosMinimos[size].sesion = "PDH";
        g_MaximosMinimos[size].tipo = 1; // Máximo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PDH encontrado: ", max_diario, " en ", TimeToString(time_max));
    }
    
    // Agregar mínimo diario
    if(min_diario < DBL_MAX)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_min;
        g_MaximosMinimos[size].maximo = 0;
        g_MaximosMinimos[size].minimo = min_diario;
        g_MaximosMinimos[size].sesion = "PDL";
        g_MaximosMinimos[size].tipo = 0; // Mínimo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PDL encontrado: ", min_diario, " en ", TimeToString(time_min));
    }
}

//+------------------------------------------------------------------+
//| Función para buscar máximo y mínimo semanal                     |
//+------------------------------------------------------------------+
void BuscarMaxMinSemanal()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Buscando máximo y mínimo de la semana pasada...");
    
    MqlDateTime dt;
    TimeToStruct(TimeCurrent(), dt);
    
    // Calcular inicio de la semana actual (lunes)
    int dias_desde_lunes = (dt.day_of_week == 0) ? 6 : dt.day_of_week - 1;
    datetime inicio_semana_actual = TimeCurrent() - dias_desde_lunes * 24 * 3600;
    TimeToStruct(inicio_semana_actual, dt);
    inicio_semana_actual = StringToTime(StringFormat("%04d.%02d.%02d 00:00", dt.year, dt.mon, dt.day));
    
    // Calcular inicio y fin de la semana pasada
    datetime inicio_semana_pasada = inicio_semana_actual - 7 * 24 * 3600;
    datetime fin_semana_pasada = inicio_semana_actual - 1; // Domingo 23:59:59
    
    double max_semanal = 0;
    double min_semanal = DBL_MAX;
    datetime time_max = 0;
    datetime time_min = 0;
    
    // Buscar en las velas de la semana pasada
    for(int i = 0; i < Bars(_Symbol, PERIOD_CURRENT); i++)
    {
        datetime vela_time = iTime(_Symbol, PERIOD_CURRENT, i);
        
        // Si la vela es anterior a la semana pasada, parar
        if(vela_time < inicio_semana_pasada) break;
        
        // Si la vela es posterior a la semana pasada, continuar
        if(vela_time > fin_semana_pasada) continue;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, i);
        double low = iLow(_Symbol, PERIOD_CURRENT, i);
        
        if(high > max_semanal)
        {
            max_semanal = high;
            time_max = vela_time;
        }
        
        if(low < min_semanal)
        {
            min_semanal = low;
            time_min = vela_time;
        }
    }
    
    // Agregar máximo semanal
    if(max_semanal > 0)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_max;
        g_MaximosMinimos[size].maximo = max_semanal;
        g_MaximosMinimos[size].minimo = 0;
        g_MaximosMinimos[size].sesion = "PWH";
        g_MaximosMinimos[size].tipo = 1; // Máximo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PWH encontrado: ", max_semanal, " en ", TimeToString(time_max));
    }
    
    // Agregar mínimo semanal
    if(min_semanal < DBL_MAX)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_min;
        g_MaximosMinimos[size].maximo = 0;
        g_MaximosMinimos[size].minimo = min_semanal;
        g_MaximosMinimos[size].sesion = "PWL";
        g_MaximosMinimos[size].tipo = 0; // Mínimo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PWL encontrado: ", min_semanal, " en ", TimeToString(time_min));
    }
}

//+------------------------------------------------------------------+
//| Función para buscar máximo y mínimo mensual                     |
//+------------------------------------------------------------------+
void BuscarMaxMinMensual()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Buscando máximo y mínimo del mes pasado...");
    
    MqlDateTime dt;
    TimeToStruct(TimeCurrent(), dt);
    
    // Calcular inicio del mes actual
    datetime inicio_mes_actual = StringToTime(StringFormat("%04d.%02d.01 00:00", dt.year, dt.mon));
    
    // Calcular mes pasado
    int mes_pasado = dt.mon - 1;
    int ano_pasado = dt.year;
    if(mes_pasado == 0)
    {
        mes_pasado = 12;
        ano_pasado = dt.year - 1;
    }
    
    // Calcular inicio y fin del mes pasado
    datetime inicio_mes_pasado = StringToTime(StringFormat("%04d.%02d.01 00:00", ano_pasado, mes_pasado));
    datetime fin_mes_pasado = inicio_mes_actual - 1; // Último segundo del mes pasado
    
    double max_mensual = 0;
    double min_mensual = DBL_MAX;
    datetime time_max = 0;
    datetime time_min = 0;
    
    // Buscar en las velas del mes pasado
    for(int i = 0; i < Bars(_Symbol, PERIOD_CURRENT); i++)
    {
        datetime vela_time = iTime(_Symbol, PERIOD_CURRENT, i);
        
        // Si la vela es anterior al mes pasado, parar
        if(vela_time < inicio_mes_pasado) break;
        
        // Si la vela es posterior al mes pasado, continuar
        if(vela_time > fin_mes_pasado) continue;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, i);
        double low = iLow(_Symbol, PERIOD_CURRENT, i);
        
        if(high > max_mensual)
        {
            max_mensual = high;
            time_max = vela_time;
        }
        
        if(low < min_mensual)
        {
            min_mensual = low;
            time_min = vela_time;
        }
    }
    
    // Agregar máximo mensual
    if(max_mensual > 0)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_max;
        g_MaximosMinimos[size].maximo = max_mensual;
        g_MaximosMinimos[size].minimo = 0;
        g_MaximosMinimos[size].sesion = "PMH";
        g_MaximosMinimos[size].tipo = 1; // Máximo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PMH encontrado: ", max_mensual, " en ", TimeToString(time_max));
    }
    
    // Agregar mínimo mensual
    if(min_mensual < DBL_MAX)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_min;
        g_MaximosMinimos[size].maximo = 0;
        g_MaximosMinimos[size].minimo = min_mensual;
        g_MaximosMinimos[size].sesion = "PML";
        g_MaximosMinimos[size].tipo = 0; // Mínimo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PML encontrado: ", min_mensual, " en ", TimeToString(time_min));
    }
}

//+------------------------------------------------------------------+
//| Función para buscar máximo y mínimo anual                       |
//+------------------------------------------------------------------+
void BuscarMaxMinAnual()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Buscando máximo y mínimo del año pasado...");
    
    MqlDateTime dt;
    TimeToStruct(TimeCurrent(), dt);
    
    // Calcular inicio del año actual y pasado
    datetime inicio_ano_actual = StringToTime(StringFormat("%04d.01.01 00:00", dt.year));
    datetime inicio_ano_pasado = StringToTime(StringFormat("%04d.01.01 00:00", dt.year - 1));
    datetime fin_ano_pasado = inicio_ano_actual - 1; // Último segundo del año pasado
    
    double max_anual = 0;
    double min_anual = DBL_MAX;
    datetime time_max = 0;
    datetime time_min = 0;
    
    // Buscar en las velas del año pasado
    for(int i = 0; i < Bars(_Symbol, PERIOD_CURRENT); i++)
    {
        datetime vela_time = iTime(_Symbol, PERIOD_CURRENT, i);
        
        // Si la vela es anterior al año pasado, parar
        if(vela_time < inicio_ano_pasado) break;
        
        // Si la vela es posterior al año pasado, continuar
        if(vela_time > fin_ano_pasado) continue;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, i);
        double low = iLow(_Symbol, PERIOD_CURRENT, i);
        
        if(high > max_anual)
        {
            max_anual = high;
            time_max = vela_time;
        }
        
        if(low < min_anual)
        {
            min_anual = low;
            time_min = vela_time;
        }
    }
    
    // Agregar máximo anual
    if(max_anual > 0)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_max;
        g_MaximosMinimos[size].maximo = max_anual;
        g_MaximosMinimos[size].minimo = 0;
        g_MaximosMinimos[size].sesion = "PYH";
        g_MaximosMinimos[size].tipo = 1; // Máximo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PYH encontrado: ", max_anual, " en ", TimeToString(time_max));
    }
    
    // Agregar mínimo anual
    if(min_anual < DBL_MAX)
    {
        int size = ArraySize(g_MaximosMinimos);
        ArrayResize(g_MaximosMinimos, size + 1);
        g_MaximosMinimos[size].time = time_min;
        g_MaximosMinimos[size].maximo = 0;
        g_MaximosMinimos[size].minimo = min_anual;
        g_MaximosMinimos[size].sesion = "PYL";
        g_MaximosMinimos[size].tipo = 0; // Mínimo
        g_MaximosMinimos[size].roto = false;
        
        if(debug) Print("PYL encontrado: ", min_anual, " en ", TimeToString(time_min));
    }
}

//+------------------------------------------------------------------+
//| Función para graficar máximos y mínimos                         |
//+------------------------------------------------------------------+
void GraficarMaximosMinimos()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Graficando máximos y mínimos...");
    
    // Limpiar líneas previas
    LimpiarLineasMaxMin();
    
    for(int i = 0; i < ArraySize(g_MaximosMinimos); i++)
    {
        string nombre_linea = "MaxMin_" + g_MaximosMinimos[i].sesion;
        string nombre_etiqueta = "Label_" + g_MaximosMinimos[i].sesion;
        
        double precio = (g_MaximosMinimos[i].tipo == 1) ? g_MaximosMinimos[i].maximo : g_MaximosMinimos[i].minimo;
        color color_linea = (g_MaximosMinimos[i].tipo == 1) ? ColorMaximo : ColorMinimo;
        
        // Obtener tiempo actual para extender la línea
        datetime tiempo_actual = TimeCurrent();
        datetime tiempo_origen = g_MaximosMinimos[i].time;
        
        // Crear línea de tendencia desde el punto específico hasta el presente
        ObjectCreate(0, nombre_linea, OBJ_TREND, 0, tiempo_origen, precio, tiempo_actual, precio);
        ObjectSetInteger(0, nombre_linea, OBJPROP_COLOR, color_linea);
        ObjectSetInteger(0, nombre_linea, OBJPROP_STYLE, STYLE_SOLID);
        ObjectSetInteger(0, nombre_linea, OBJPROP_WIDTH, 2);
        ObjectSetInteger(0, nombre_linea, OBJPROP_BACK, false);
        ObjectSetInteger(0, nombre_linea, OBJPROP_SELECTABLE, false);
        ObjectSetInteger(0, nombre_linea, OBJPROP_HIDDEN, true);
        ObjectSetInteger(0, nombre_linea, OBJPROP_RAY_RIGHT, true); // Extender hacia la derecha
        ObjectSetInteger(0, nombre_linea, OBJPROP_RAY_LEFT, false); // No extender hacia la izquierda
        
        // Crear etiqueta en el punto de origen
        ObjectCreate(0, nombre_etiqueta, OBJ_TEXT, 0, tiempo_origen, precio);
        ObjectSetString(0, nombre_etiqueta, OBJPROP_TEXT, g_MaximosMinimos[i].sesion);
        ObjectSetInteger(0, nombre_etiqueta, OBJPROP_COLOR, color_linea);
        ObjectSetInteger(0, nombre_etiqueta, OBJPROP_FONTSIZE, 10);
        ObjectSetString(0, nombre_etiqueta, OBJPROP_FONT, "Arial Bold");
        ObjectSetInteger(0, nombre_etiqueta, OBJPROP_ANCHOR, ANCHOR_LEFT_UPPER);
        ObjectSetInteger(0, nombre_etiqueta, OBJPROP_SELECTABLE, false);
        ObjectSetInteger(0, nombre_etiqueta, OBJPROP_HIDDEN, true);
        
        if(debug) Print("Graficado: ", g_MaximosMinimos[i].sesion, " desde ", TimeToString(tiempo_origen), " en precio: ", precio);
    }
}

//+------------------------------------------------------------------+
//| Función para limpiar líneas de máximos y mínimos                |
//+------------------------------------------------------------------+
void LimpiarLineasMaxMin()
{
    string prefijos[] = {"MaxMin_", "Label_"};
    string sufijos[] = {"PDH", "PDL", "PWH", "PWL", "PMH", "PML", "PYH", "PYL"};
    
    for(int p = 0; p < ArraySize(prefijos); p++)
    {
        for(int s = 0; s < ArraySize(sufijos); s++)
        {
            string nombre = prefijos[p] + sufijos[s];
            ObjectDelete(0, nombre);
        }
    }
}

//+------------------------------------------------------------------+
//| Función para evaluar convergencias y operar                     |
//+------------------------------------------------------------------+
void EvaluarConvergenciasMaxMin()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Evaluando convergencias de máximos y mínimos...");
    
    // Separar máximos y mínimos
    double maximos[];
    double minimos[];
    int puntuacion_maximos[];
    int puntuacion_minimos[];
    
    ArrayResize(maximos, 0);
    ArrayResize(minimos, 0);
    ArrayResize(puntuacion_maximos, 0);
    ArrayResize(puntuacion_minimos, 0);
    
    // Clasificar y asignar puntuaciones
    for(int i = 0; i < ArraySize(g_MaximosMinimos); i++)
    {
        if(g_MaximosMinimos[i].tipo == 1) // Máximo
        {
            int size = ArraySize(maximos);
            ArrayResize(maximos, size + 1);
            ArrayResize(puntuacion_maximos, size + 1);
            maximos[size] = g_MaximosMinimos[i].maximo;
            
            // Asignar puntuación según tipo
            if(g_MaximosMinimos[i].sesion == "PYH") puntuacion_maximos[size] = 4; // Anual
            else if(g_MaximosMinimos[i].sesion == "PMH") puntuacion_maximos[size] = 3; // Mensual
            else if(g_MaximosMinimos[i].sesion == "PWH") puntuacion_maximos[size] = 2; // Semanal
            else if(g_MaximosMinimos[i].sesion == "PDH") puntuacion_maximos[size] = 1; // Diario
        }
        else // Mínimo
        {
            int size = ArraySize(minimos);
            ArrayResize(minimos, size + 1);
            ArrayResize(puntuacion_minimos, size + 1);
            minimos[size] = g_MaximosMinimos[i].minimo;
            
            // Asignar puntuación según tipo
            if(g_MaximosMinimos[i].sesion == "PYL") puntuacion_minimos[size] = 4; // Anual
            else if(g_MaximosMinimos[i].sesion == "PML") puntuacion_minimos[size] = 3; // Mensual
            else if(g_MaximosMinimos[i].sesion == "PWL") puntuacion_minimos[size] = 2; // Semanal
            else if(g_MaximosMinimos[i].sesion == "PDL") puntuacion_minimos[size] = 1; // Diario
        }
    }
    
    // Evaluar convergencias en máximos
    EvaluarConvergenciaMaximos(maximos, puntuacion_maximos);
    
    // Evaluar convergencias en mínimos
    EvaluarConvergenciaMinimos(minimos, puntuacion_minimos);
}

//+------------------------------------------------------------------+
//| Función para evaluar convergencia de máximos                    |
//+------------------------------------------------------------------+
void EvaluarConvergenciaMaximos(double &maximos[], int &puntuaciones[])
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Evaluando convergencia de máximos...");
    
    if(ArraySize(maximos) < 2) return; // Necesitamos al menos 2 máximos
    
    double rango_convergencia = 200 * _Point; // 200 puntos
    
    // Buscar grupos de máximos convergentes
    for(int i = 0; i < ArraySize(maximos); i++)
    {
        double maximos_convergentes[];
        int puntuaciones_convergentes[];
        
        ArrayResize(maximos_convergentes, 0);
        ArrayResize(puntuaciones_convergentes, 0);
        
        // Agregar el máximo actual
        ArrayResize(maximos_convergentes, 1);
        ArrayResize(puntuaciones_convergentes, 1);
        maximos_convergentes[0] = maximos[i];
        puntuaciones_convergentes[0] = puntuaciones[i];
        
        // Buscar otros máximos dentro del rango
        for(int j = i + 1; j < ArraySize(maximos); j++)
        {
            if(MathAbs(maximos[i] - maximos[j]) <= rango_convergencia)
            {
                int size = ArraySize(maximos_convergentes);
                ArrayResize(maximos_convergentes, size + 1);
                ArrayResize(puntuaciones_convergentes, size + 1);
                maximos_convergentes[size] = maximos[j];
                puntuaciones_convergentes[size] = puntuaciones[j];
            }
        }
        
        // Si hay convergencia (al menos 2 máximos), operar en el de mayor puntuación
        if(ArraySize(maximos_convergentes) >= 2)
        {
            // Encontrar el máximo con mayor puntuación
            int max_puntuacion = 0;
            double precio_operacion = 0;
            
            for(int k = 0; k < ArraySize(puntuaciones_convergentes); k++)
            {
                if(puntuaciones_convergentes[k] > max_puntuacion)
                {
                    max_puntuacion = puntuaciones_convergentes[k];
                    precio_operacion = maximos_convergentes[k];
                }
            }
            
            if(debug) Print("Convergencia de máximos detectada. Precio operación: ", precio_operacion, " Puntuación: ", max_puntuacion);
            
            // Colocar orden de venta
            ColocarOrdenMaxMin(ORDER_TYPE_SELL_LIMIT, precio_operacion, "Convergencia Máximos");
            
            break; // Solo operar una convergencia por tick
        }
    }
}

//+------------------------------------------------------------------+
//| Función para evaluar convergencia de mínimos                    |
//+------------------------------------------------------------------+
void EvaluarConvergenciaMinimos(double &minimos[], int &puntuaciones[])
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Evaluando convergencia de mínimos...");
    
    if(ArraySize(minimos) < 2) return; // Necesitamos al menos 2 mínimos
    
    double rango_convergencia = 200 * _Point; // 200 puntos
    
    // Buscar grupos de mínimos convergentes
    for(int i = 0; i < ArraySize(minimos); i++)
    {
        double minimos_convergentes[];
        int puntuaciones_convergentes[];
        
        ArrayResize(minimos_convergentes, 0);
        ArrayResize(puntuaciones_convergentes, 0);
        
        // Agregar el mínimo actual
        ArrayResize(minimos_convergentes, 1);
        ArrayResize(puntuaciones_convergentes, 1);
        minimos_convergentes[0] = minimos[i];
        puntuaciones_convergentes[0] = puntuaciones[i];
        
        // Buscar otros mínimos dentro del rango
        for(int j = i + 1; j < ArraySize(minimos); j++)
        {
            if(MathAbs(minimos[i] - minimos[j]) <= rango_convergencia)
            {
                int size = ArraySize(minimos_convergentes);
                ArrayResize(minimos_convergentes, size + 1);
                ArrayResize(puntuaciones_convergentes, size + 1);
                minimos_convergentes[size] = minimos[j];
                puntuaciones_convergentes[size] = puntuaciones[j];
            }
        }
        
        // Si hay convergencia (al menos 2 mínimos), operar en el de mayor puntuación
        if(ArraySize(minimos_convergentes) >= 2)
        {
            // Encontrar el mínimo con mayor puntuación
            int max_puntuacion = 0;
            double precio_operacion = 0;
            
            for(int k = 0; k < ArraySize(puntuaciones_convergentes); k++)
            {
                if(puntuaciones_convergentes[k] > max_puntuacion)
                {
                    max_puntuacion = puntuaciones_convergentes[k];
                    precio_operacion = minimos_convergentes[k];
                }
            }
            
            if(debug) Print("Convergencia de mínimos detectada. Precio operación: ", precio_operacion, " Puntuación: ", max_puntuacion);
            
            // Colocar orden de compra
            ColocarOrdenMaxMin(ORDER_TYPE_BUY_LIMIT, precio_operacion, "Convergencia Mínimos");
            
            break; // Solo operar una convergencia por tick
        }
    }
}

//+------------------------------------------------------------------+
//| Función para colocar orden en máximos/mínimos                   |
//+------------------------------------------------------------------+
void ColocarOrdenMaxMin(ENUM_ORDER_TYPE tipo, double precio, string comentario)
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Colocando orden MaxMin: ", EnumToString(tipo), " en precio: ", precio);
    
    // Verificar si ya existe una orden cerca de este precio
    if(ExisteOrdenCercana(precio, DistanciaMinimaEntreOrdenes))
    {
        if(debug) Print("Ya existe una orden cerca del precio: ", precio);
        return;
    }
    
    // Verificar distancia al precio actual
    double precio_actual = (tipo == ORDER_TYPE_BUY_LIMIT) ? SymbolInfoDouble(_Symbol, SYMBOL_ASK) : SymbolInfoDouble(_Symbol, SYMBOL_BID);
    double distancia = MathAbs(precio - precio_actual) / _Point;
    
    if(distancia > DistanciaMaximaAlPuntoEntrada)
    {
        if(debug) Print("Precio muy alejado del actual. Distancia: ", distancia, " puntos");
        return;
    }
    
    // Calcular lotaje
    double lotaje = CalcularLotaje();
    if(lotaje <= 0)
    {
        if(debug) Print("Error calculando lotaje");
        return;
    }
    
    // Calcular Stop Loss y Take Profit
    double stop_loss = 0;
    double take_profit = 0;
    
    if(tipo == ORDER_TYPE_BUY_LIMIT)
    {
        stop_loss = precio - (StopLossPoints + StopLossBuffer) * _Point;
        take_profit = precio + (StopLossPoints + StopLossBuffer) * RiesgoRecompensa * _Point;
    }
    else
    {
        stop_loss = precio + (StopLossPoints + StopLossBuffer) * _Point;
        take_profit = precio - (StopLossPoints + StopLossBuffer) * RiesgoRecompensa * _Point;
    }
    
    // Colocar la orden
    MqlTradeRequest request = {};
    MqlTradeResult result = {};
    
    request.action = TRADE_ACTION_PENDING;
    request.symbol = _Symbol;
    request.volume = lotaje;
    request.type = tipo;
    request.price = precio;
    request.sl = stop_loss;
    request.tp = take_profit;
    request.comment = comentario;
    request.magic = 12345;
    
    if(OrderSend(request, result))
    {
        if(debug) Print("Orden MaxMin colocada exitosamente. Ticket: ", result.order);
        g_TradesPorSesion++;
    }
    else
    {
        if(debug) Print("Error colocando orden MaxMin: ", result.retcode, " - ", result.comment);
    }
}

//+------------------------------------------------------------------+
//| Función para verificar conflictos entre Imbalances y OrderBlocks|
//+------------------------------------------------------------------+
void VerificarConflictosImbalanceOB()
{
    bool debug = g_DebugMode && DebugImbalances;
    if(debug) Print("Verificando conflictos entre Imbalances y OrderBlocks...");
    
    for(int i = 0; i < ArraySize(g_Imbalances); i++)
    {
        if(!g_Imbalances[i].operable || g_Imbalances[i].mitigado) continue;
        
        // Verificar distancia a OrderBlocks
        for(int j = 0; j < ArraySize(g_OrderBlocks); j++)
        {
            if(!g_OrderBlocks[j].operable || g_OrderBlocks[j].mitigado) continue;
            
            double distancia = MathAbs(g_Imbalances[i].punto_medio - g_OrderBlocks[j].punto_medio) / _Point;
            
            if(distancia <= DistanciaImbalanceOB)
            {
                // OrderBlock tiene prioridad, desactivar Imbalance
                g_Imbalances[i].operable = false;
                
                if(debug) Print("Imbalance ", g_Imbalances[i].id, " desactivado por proximidad a OrderBlock ", g_OrderBlocks[j].id, ". Distancia: ", distancia, " puntos");
                
                break;
            }
        }
    }
}

//+------------------------------------------------------------------+
//| Función para verificar conflictos con OrderBlocks (Nueva Lógica)|
//+------------------------------------------------------------------+
void VerificarConflictosOBMaxMinNuevo()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Verificando conflictos entre OrderBlocks y MaxMin con nueva lógica...");
    
    for(int i = 0; i < ArraySize(g_OrderBlocks); i++)
    {
        if(!g_OrderBlocks[i].operable || g_OrderBlocks[i].mitigado) continue;
        
        bool maxMinEncontrado = false;
        
        // Verificar según el tipo de OrderBlock
        for(int j = 0; j < ArraySize(g_MaximosMinimos); j++)
        {
            if(g_MaximosMinimos[j].roto) continue;
            
            double precio_maxmin = (g_MaximosMinimos[j].tipo == 1) ? g_MaximosMinimos[j].maximo : g_MaximosMinimos[j].minimo;
            double distancia = MathAbs(g_OrderBlocks[i].punto_medio - precio_maxmin) / _Point;
            
            // Lógica nueva: verificar dirección y proximidad
            if(distancia <= DistanciaOBMaxMin)
            {
                // Para OrderBlock Alcista: buscar Low (mínimo) por debajo
                if(g_OrderBlocks[i].tipo == OB_ALCISTA && g_MaximosMinimos[j].tipo == 0 && precio_maxmin < g_OrderBlocks[i].punto_medio)
                {
                    // MaxMin (Low) tiene prioridad, desactivar OB
                    g_OrderBlocks[i].mitigado = true;
                    g_OrderBlocks[i].operable = false;
                    maxMinEncontrado = true;
                    
                    if(debug) Print("OrderBlock Alcista ", g_OrderBlocks[i].id, " desactivado por Low ", g_MaximosMinimos[j].sesion, " por debajo. Distancia: ", distancia, " puntos");
                    break;
                }
                // Para OrderBlock Bajista: buscar High (máximo) por encima
                else if(g_OrderBlocks[i].tipo == OB_BAJISTA && g_MaximosMinimos[j].tipo == 1 && precio_maxmin > g_OrderBlocks[i].punto_medio)
                {
                    // MaxMin (High) tiene prioridad, desactivar OB
                    g_OrderBlocks[i].mitigado = true;
                    g_OrderBlocks[i].operable = false;
                    maxMinEncontrado = true;
                    
                    if(debug) Print("OrderBlock Bajista ", g_OrderBlocks[i].id, " desactivado por High ", g_MaximosMinimos[j].sesion, " por encima. Distancia: ", distancia, " puntos");
                    break;
                }
            }
        }
        
        if(!maxMinEncontrado && debug)
        {
            Print("OrderBlock ", g_OrderBlocks[i].id, " mantiene prioridad - no hay MaxMin en dirección correcta cerca");
        }
    }
}

//+------------------------------------------------------------------+
//| Función para verificar conflictos con OrderBlocks (Lógica Antigua)|
//+------------------------------------------------------------------+
void VerificarConflictosOBMaxMin()
{
    bool debug = g_DebugMode && DebugMaxMin;
    if(debug) Print("Verificando conflictos entre OrderBlocks y MaxMin...");
    
    for(int i = 0; i < ArraySize(g_OrderBlocks); i++)
    {
        if(!g_OrderBlocks[i].operable || g_OrderBlocks[i].mitigado) continue;
        
        // Verificar distancia a máximos y mínimos
        for(int j = 0; j < ArraySize(g_MaximosMinimos); j++)
        {
            double precio_maxmin = (g_MaximosMinimos[j].tipo == 1) ? g_MaximosMinimos[j].maximo : g_MaximosMinimos[j].minimo;
            double distancia = MathAbs(g_OrderBlocks[i].punto_medio - precio_maxmin) / _Point;
            
            if(distancia <= DistanciaOBMaxMin)
            {
                // MaxMin tiene prioridad, marcar OB como mitigado
                g_OrderBlocks[i].mitigado = true;
                g_OrderBlocks[i].operable = false;
                
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " marcado como mitigado por proximidad a MaxMin. Distancia: ", distancia, " puntos");
                
                break;
            }
        }
    }
}

//+------------------------------------------------------------------+
//| Función para aplicar confirmaciones a OrderBlocks               |
//+------------------------------------------------------------------+
void AplicarConfirmacionesOB()
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Aplicando confirmaciones a OrderBlocks...");
    
    for(int i = 0; i < ArraySize(g_OrderBlocks); i++)
    {
        if(debug) Print("Aplicando confirmaciones a OB: ", g_OrderBlocks[i].id);
        
        // Aplicar confirmación de confluencia
        if(Confluencia)
        {
            g_OrderBlocks[i].confluencia = DetectarConfluencia(g_OrderBlocks[i]);
            if(debug) Print("  - Confluencia: ", g_OrderBlocks[i].confluencia);
        }
        
        // Aplicar confirmación de break of structure
        if(BreakofStructure)
        {
            g_OrderBlocks[i].breakofstructure = BreakOfStructure(g_OrderBlocks[i]);
            if(debug) Print("  - Break of Structure: ", g_OrderBlocks[i].breakofstructure);
        }
        
        // Aplicar confirmación de liquidez previa
        if(LiqPrevOB)
        {
            g_OrderBlocks[i].liquidez_previa = DetectarLiquidezPrevia(g_OrderBlocks[i]);
            if(debug) Print("  - Liquidez Previa: ", g_OrderBlocks[i].liquidez_previa);
        }
        
        // Verificar si el OB sigue siendo operable después de las confirmaciones
        bool todas_confirmaciones = true;
        
        if(Confluencia && !g_OrderBlocks[i].confluencia) todas_confirmaciones = false;
        if(BreakofStructure && !g_OrderBlocks[i].breakofstructure) todas_confirmaciones = false;
        if(LiqPrevOB && !g_OrderBlocks[i].liquidez_previa) todas_confirmaciones = false;
        
        // Solo es operable si pasa todas las confirmaciones Y no está mitigado Y no está operado
        g_OrderBlocks[i].operable = todas_confirmaciones && !g_OrderBlocks[i].mitigado && !g_OrderBlocks[i].operado;
        
        if(debug) Print("  - OB operable después de confirmaciones: ", g_OrderBlocks[i].operable);
    }
    
    if(debug) Print("Confirmaciones aplicadas a todos los OrderBlocks");
}

//+------------------------------------------------------------------+
//| Función para detectar confluencia en OrderBlock                 |
//+------------------------------------------------------------------+
bool DetectarConfluencia(OrderBlock &ob)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Detectando confluencia para OB: ", ob.id);
    
    // Calcular cuántas velas revisar hacia atrás (DiasConfluencia * velas por día)
    int velas_por_dia = 24; // Asumiendo timeframe H1
    if(PERIOD_CURRENT == PERIOD_M15) velas_por_dia = 96;
    else if(PERIOD_CURRENT == PERIOD_M30) velas_por_dia = 48;
    else if(PERIOD_CURRENT == PERIOD_H4) velas_por_dia = 6;
    else if(PERIOD_CURRENT == PERIOD_D1) velas_por_dia = 1;
    
    int velas_revisar = DiasConfluencia * velas_por_dia;
    int vela_inicio_busqueda = ob.vela_inicio + velas_revisar;
    
    // Limitar al número de velas disponibles
    int total_velas = Bars(_Symbol, PERIOD_CURRENT);
    if(vela_inicio_busqueda >= total_velas)
        vela_inicio_busqueda = total_velas - 1;
    
    // Definir el rango del OrderBlock como zona de soporte/resistencia
    double rango_superior = ob.high;
    double rango_inferior = ob.low;
    double tolerancia_ruptura = _Point * 15; // Tolerancia para considerar ruptura significativa
    
    int toques_sin_ruptura = 0;
    
    if(debug) 
    {
        Print("Analizando rango OB: ", rango_inferior, " - ", rango_superior);
        Print("Revisando ", velas_revisar, " velas hacia atrás desde vela ", ob.vela_inicio);
    }
    
    // Revisar velas históricas anteriores a la creación del OB
    for(int i = ob.vela_inicio + 1; i <= vela_inicio_busqueda; i++)
    {
        double high = iHigh(_Symbol, PERIOD_CURRENT, i);
        double low = iLow(_Symbol, PERIOD_CURRENT, i);
        double open = iOpen(_Symbol, PERIOD_CURRENT, i);
        double close = iClose(_Symbol, PERIOD_CURRENT, i);
        
        bool toque_detectado = false;
        bool ruptura_significativa = false;
        
        // Verificar si el precio interactuó con el rango del OB
        bool precio_en_rango = (low <= rango_superior && high >= rango_inferior);
        
        if(precio_en_rango)
        {
            // Verificar si hubo ruptura significativa (cierre fuera del rango)
            if(close > rango_superior + tolerancia_ruptura || close < rango_inferior - tolerancia_ruptura)
            {
                ruptura_significativa = true;
                if(debug) Print("Ruptura significativa en vela ", i, " - Close: ", close, " fuera del rango");
            }
            else
            {
                // Es un toque sin ruptura significativa
                toque_detectado = true;
                
                // Verificar el tipo de interacción
                bool rebote_superior = (high >= rango_superior - _Point * 5 && close < rango_superior);
                bool rebote_inferior = (low <= rango_inferior + _Point * 5 && close > rango_inferior);
                bool penetracion_controlada = (low < rango_inferior && high > rango_superior && 
                                             close >= rango_inferior && close <= rango_superior);
                
                if(rebote_superior || rebote_inferior || penetracion_controlada)
                {
                    toques_sin_ruptura++;
                    if(debug) 
                    {
                        string tipo_toque = rebote_superior ? "Rebote Superior" : 
                                          rebote_inferior ? "Rebote Inferior" : "Penetración Controlada";
                        Print("Toque sin ruptura detectado en vela ", i, " - Tipo: ", tipo_toque);
                    }
                }
            }
        }
        
        // Si hay ruptura significativa, el análisis se invalida
        if(ruptura_significativa)
        {
            if(debug) Print("Confluencia invalidada por ruptura significativa en vela ", i);
            return false;
        }
    }
    
    // Verificar si cumple el criterio mínimo de toques
    bool hay_confluencia = (toques_sin_ruptura >= ToquesMinConfluencia);
    
    if(debug)
    {
        Print("Resultado confluencia:");
        Print("  - Toques sin ruptura detectados: ", toques_sin_ruptura);
        Print("  - Toques mínimos requeridos: ", ToquesMinConfluencia);
        Print("  - Hay confluencia: ", hay_confluencia);
    }
    
    return hay_confluencia;
}

//+------------------------------------------------------------------+
//| Función para detectar Break of Structure                        |
//+------------------------------------------------------------------+
bool BreakOfStructure(OrderBlock &ob)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Detectando Break of Structure para OB: ", ob.id);
    
    // Buscar máximo y mínimo en las 20 velas anteriores al OB
    double max_high = 0;
    double min_low = DBL_MAX;
    int velas_contexto = 20;
    
    for(int i = ob.vela_inicio + 1; i <= ob.vela_inicio + velas_contexto; i++)
    {
        if(i >= Bars(_Symbol, PERIOD_CURRENT)) break;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, i);
        double low = iLow(_Symbol, PERIOD_CURRENT, i);
        
        if(high > max_high) max_high = high;
        if(low < min_low) min_low = low;
    }
    
    if(debug) Print("Contexto - Max High: ", max_high, ", Min Low: ", min_low);
    
    // Revisar las siguientes 5 velas después del OB
    for(int i = 1; i <= 5; i++)
    {
        int vela_check = ob.vela_inicio - i; // Velas más recientes
        if(vela_check < 0) break;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, vela_check);
        double low = iLow(_Symbol, PERIOD_CURRENT, vela_check);
        
        // Para OB Alcista: buscar ruptura del máximo anterior
        if(ob.tipo == 1 && high > max_high)
        {
            if(debug) Print("Break of Structure ALCISTA detectado en vela ", vela_check, " - High: ", high, " > Max anterior: ", max_high);
            return true;
        }
        
        // Para OB Bajista: buscar ruptura del mínimo anterior
        if(ob.tipo == 0 && low < min_low)
        {
            if(debug) Print("Break of Structure BAJISTA detectado en vela ", vela_check, " - Low: ", low, " < Min anterior: ", min_low);
            return true;
        }
    }
    
    if(debug) Print("No se detectó Break of Structure");
    return false;
}

//+------------------------------------------------------------------+
//| Función para detectar Liquidez Previa                          |
//+------------------------------------------------------------------+
bool DetectarLiquidezPrevia(OrderBlock &ob)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Detectando Liquidez Previa para OB: ", ob.id);
    
    // Revisar las últimas 6 horas (6 velas en H1, 24 velas en M15, etc.)
    int velas_6h = 6;
    if(PERIOD_CURRENT == PERIOD_M15) velas_6h = 24;
    else if(PERIOD_CURRENT == PERIOD_M30) velas_6h = 12;
    else if(PERIOD_CURRENT == PERIOD_H4) velas_6h = 2;
    
    double max_high = 0;
    double min_low = DBL_MAX;
    
    // Obtener rango de las últimas 6 horas antes del OB
    for(int i = 1; i <= velas_6h; i++)
    {
        int vela_check = ob.vela_inicio + i;
        if(vela_check >= Bars(_Symbol, PERIOD_CURRENT)) break;
        
        double high = iHigh(_Symbol, PERIOD_CURRENT, vela_check);
        double low = iLow(_Symbol, PERIOD_CURRENT, vela_check);
        
        if(high > max_high) max_high = high;
        if(low < min_low) min_low = low;
    }
    
    if(max_high == 0 || min_low == DBL_MAX)
    {
        if(debug) Print("No hay datos suficientes para evaluar liquidez previa");
        return false;
    }
    
    double rango_6h = max_high - min_low;
    double rango_ob = ob.high - ob.low;
    
    // Verificar si el OB está dentro del rango de acumulación (las 6 horas previas)
    bool ob_en_rango = (ob.high <= max_high && ob.low >= min_low);
    
    // Verificar si el rango de 6h es relativamente pequeño (acumulación)
    // El rango de acumulación debe ser menor a 3 veces el tamaño del OB
    bool es_acumulacion = (rango_6h <= rango_ob * 3);
    
    bool hay_liquidez = ob_en_rango && es_acumulacion;
    
    if(debug)
    {
        Print("Análisis Liquidez Previa:");
        Print("  - Rango 6h: ", rango_6h, " (High: ", max_high, ", Low: ", min_low, ")");
        Print("  - Rango OB: ", rango_ob);
        Print("  - OB en rango: ", ob_en_rango);
        Print("  - Es acumulación: ", es_acumulacion);
        Print("  - Hay liquidez previa: ", hay_liquidez);
    }
    
    return hay_liquidez;
}

//+------------------------------------------------------------------+
//| Función para detectar Inducción                                 |
//+------------------------------------------------------------------+
bool DetectarInduccion(OrderBlock &ob)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Detectando inducción para OB: ", ob.id);
    
    // Analizar las 10 velas anteriores al OB para detectar inducción
    int velas_analizar = 10;
    bool induccion_detectada = false;
    
    if(ob.tipo == 1) // OB Alcista - buscar inducción bajista previa
    {
        // Buscar un mínimo que fue roto antes del OB
        double minimo_previo = iLow(_Symbol, PERIOD_CURRENT, ob.vela_inicio + 1);
        int indice_minimo = ob.vela_inicio + 1;
        
        // Encontrar el mínimo más bajo en las velas anteriores
        for(int i = ob.vela_inicio + 2; i <= ob.vela_inicio + velas_analizar && i < Bars(_Symbol, PERIOD_CURRENT); i++)
        {
            double low = iLow(_Symbol, PERIOD_CURRENT, i);
            if(low < minimo_previo)
            {
                minimo_previo = low;
                indice_minimo = i;
            }
        }
        
        // Verificar si el precio rompió este mínimo y luego se recuperó
        for(int i = indice_minimo - 1; i > ob.vela_inicio; i--)
        {
            double low = iLow(_Symbol, PERIOD_CURRENT, i);
            if(low < minimo_previo)
            {
                // Se rompió el mínimo, ahora verificar recuperación
                for(int j = i - 1; j >= ob.vela_inicio; j--)
                {
                    double close = iClose(_Symbol, PERIOD_CURRENT, j);
                    if(close > minimo_previo)
                    {
                        induccion_detectada = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    else if(ob.tipo == 0) // OB Bajista - buscar inducción alcista previa
    {
        // Buscar un máximo que fue roto antes del OB
        double maximo_previo = iHigh(_Symbol, PERIOD_CURRENT, ob.vela_inicio + 1);
        int indice_maximo = ob.vela_inicio + 1;
        
        // Encontrar el máximo más alto en las velas anteriores
        for(int i = ob.vela_inicio + 2; i <= ob.vela_inicio + velas_analizar && i < Bars(_Symbol, PERIOD_CURRENT); i++)
        {
            double high = iHigh(_Symbol, PERIOD_CURRENT, i);
            if(high > maximo_previo)
            {
                maximo_previo = high;
                indice_maximo = i;
            }
        }
        
        // Verificar si el precio rompió este máximo y luego se recuperó
        for(int i = indice_maximo - 1; i > ob.vela_inicio; i--)
        {
            double high = iHigh(_Symbol, PERIOD_CURRENT, i);
            if(high > maximo_previo)
            {
                // Se rompió el máximo, ahora verificar recuperación
                for(int j = i - 1; j >= ob.vela_inicio; j--)
                {
                    double close = iClose(_Symbol, PERIOD_CURRENT, j);
                    if(close < maximo_previo)
                    {
                        induccion_detectada = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    
    if(debug)
    {
        Print("Análisis de inducción para OB ", ob.id, ":");
        Print("  - Tipo OB: ", (ob.tipo == 1 ? "Alcista" : "Bajista"));
        Print("  - Inducción detectada: ", induccion_detectada);
    }
    
    return induccion_detectada;
}

//+------------------------------------------------------------------+
//| Función para detectar Patrón MW (Market Maker Move)             |
//+------------------------------------------------------------------+
bool DetectarPatronMW(OrderBlock &ob)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Detectando patrón MW para OB: ", ob.id);
    
    // Patrón MW: Buscar una secuencia de movimiento-consolidación-movimiento
    // Analizar las 15 velas anteriores al OB
    int velas_analizar = 15;
    bool patron_mw = false;
    
    if(ob.tipo == 1) // OB Alcista - buscar patrón MW alcista
    {
        // Buscar: Movimiento bajista -> Consolidación -> Movimiento alcista (que forma el OB)
        
        // 1. Identificar movimiento bajista inicial
        double precio_inicio = iHigh(_Symbol, PERIOD_CURRENT, ob.vela_inicio + velas_analizar);
        double precio_minimo = precio_inicio;
        int indice_minimo = ob.vela_inicio + velas_analizar;
        
        // Encontrar el punto más bajo del movimiento bajista
        for(int i = ob.vela_inicio + velas_analizar - 1; i > ob.vela_inicio + 5; i--)
        {
            double low = iLow(_Symbol, PERIOD_CURRENT, i);
            if(low < precio_minimo)
            {
                precio_minimo = low;
                indice_minimo = i;
            }
        }
        
        // 2. Verificar consolidación (rango estrecho después del mínimo)
        double rango_consolidacion = 0;
        int velas_consolidacion = 0;
        
        for(int i = indice_minimo - 1; i > ob.vela_inicio; i--)
        {
            double high = iHigh(_Symbol, PERIOD_CURRENT, i);
            double low = iLow(_Symbol, PERIOD_CURRENT, i);
            rango_consolidacion += (high - low);
            velas_consolidacion++;
        }
        
        if(velas_consolidacion > 0)
        {
            double rango_promedio = rango_consolidacion / velas_consolidacion;
            double movimiento_inicial = precio_inicio - precio_minimo;
            
            // 3. Verificar que el OB representa un movimiento alcista significativo
            double movimiento_ob = ob.high - ob.low;
            
            // Condiciones para patrón MW:
            // - Movimiento inicial bajista significativo
            // - Consolidación con rango promedio pequeño
            // - OB con movimiento alcista
            bool movimiento_significativo = movimiento_inicial > (30 * _Point);
            bool consolidacion_estrecha = rango_promedio < (movimiento_inicial * 0.3);
            bool ob_alcista_fuerte = movimiento_ob > (rango_promedio * 1.5);
            
            patron_mw = movimiento_significativo && consolidacion_estrecha && ob_alcista_fuerte;
            
            if(debug)
            {
                Print("Análisis MW Alcista para OB ", ob.id, ":");
                Print("  - Movimiento inicial: ", DoubleToString(movimiento_inicial/_Point, 1), " pips");
                Print("  - Rango consolidación: ", DoubleToString(rango_promedio/_Point, 1), " pips");
                Print("  - Movimiento OB: ", DoubleToString(movimiento_ob/_Point, 1), " pips");
                Print("  - Patrón MW detectado: ", patron_mw);
            }
        }
    }
    else if(ob.tipo == 0) // OB Bajista - buscar patrón MW bajista
    {
        // Buscar: Movimiento alcista -> Consolidación -> Movimiento bajista (que forma el OB)
        
        // 1. Identificar movimiento alcista inicial
        double precio_inicio = iLow(_Symbol, PERIOD_CURRENT, ob.vela_inicio + velas_analizar);
        double precio_maximo = precio_inicio;
        int indice_maximo = ob.vela_inicio + velas_analizar;
        
        // Encontrar el punto más alto del movimiento alcista
        for(int i = ob.vela_inicio + velas_analizar - 1; i > ob.vela_inicio + 5; i--)
        {
            double high = iHigh(_Symbol, PERIOD_CURRENT, i);
            if(high > precio_maximo)
            {
                precio_maximo = high;
                indice_maximo = i;
            }
        }
        
        // 2. Verificar consolidación
        double rango_consolidacion = 0;
        int velas_consolidacion = 0;
        
        for(int i = indice_maximo - 1; i > ob.vela_inicio; i--)
        {
            double high = iHigh(_Symbol, PERIOD_CURRENT, i);
            double low = iLow(_Symbol, PERIOD_CURRENT, i);
            rango_consolidacion += (high - low);
            velas_consolidacion++;
        }
        
        if(velas_consolidacion > 0)
        {
            double rango_promedio = rango_consolidacion / velas_consolidacion;
            double movimiento_inicial = precio_maximo - precio_inicio;
            
            // 3. Verificar que el OB representa un movimiento bajista significativo
            double movimiento_ob = ob.high - ob.low;
            
            // Condiciones para patrón MW bajista
            bool movimiento_significativo = movimiento_inicial > (30 * _Point);
            bool consolidacion_estrecha = rango_promedio < (movimiento_inicial * 0.3);
            bool ob_bajista_fuerte = movimiento_ob > (rango_promedio * 1.5);
            
            patron_mw = movimiento_significativo && consolidacion_estrecha && ob_bajista_fuerte;
            
            if(debug)
            {
                Print("Análisis MW Bajista para OB ", ob.id, ":");
                Print("  - Movimiento inicial: ", DoubleToString(movimiento_inicial/_Point, 1), " pips");
                Print("  - Rango consolidación: ", DoubleToString(rango_promedio/_Point, 1), " pips");
                Print("  - Movimiento OB: ", DoubleToString(movimiento_ob/_Point, 1), " pips");
                Print("  - Patrón MW detectado: ", patron_mw);
            }
        }
    }
    
    return patron_mw;
}

//+------------------------------------------------------------------+
//| Función para graficar OrderBlocks                               |
//+------------------------------------------------------------------+
void GraficarOrderBlocks()
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Graficando OrderBlocks...");
    
    // Eliminar objetos existentes de OrderBlocks para optimizar
    for(int obj = ObjectsTotal(0, 0, OBJ_RECTANGLE) - 1; obj >= 0; obj--)
    {
        string name = ObjectName(0, obj, 0, OBJ_RECTANGLE);
        if(StringFind(name, "OB_") == 0)
        {
            ObjectDelete(0, name);
        }
    }
    
    // Eliminar líneas de punto medio existentes
    for(int obj = ObjectsTotal(0, 0, OBJ_HLINE) - 1; obj >= 0; obj--)
    {
        string name = ObjectName(0, obj, 0, OBJ_HLINE);
        if(StringFind(name, "OB_MID_") == 0)
        {
            ObjectDelete(0, name);
        }
    }
    
    // Solo graficar OrderBlocks activos (no mitigados y no operados)
    for(int i = 0; i < ArraySize(g_OrderBlocks); i++)
    {
        OrderBlock ob = g_OrderBlocks[i];
        
        // No graficar OrderBlocks que están mitigados Y operados
        if(ob.mitigado && ob.operado)
        {
            if(debug) Print("OrderBlock ", ob.id, " no se grafica: mitigado y operado");
            continue;
        }
        
        // No graficar OrderBlocks que excedan el tamaño máximo
        double tamanoOB = ob.tamano / _Point;
        if(tamanoOB >= TamanoMaximoOB)
        {
            if(debug) Print("OrderBlock ", ob.id, " no se grafica: tamaño excede máximo (", tamanoOB, " >= ", TamanoMaximoOB, ")");
            continue;
        }
        
        // Graficar OrderBlocks activos y recientemente mitigados (pero no operados)
        // Los mitigados se mostrarán con estilo diferente
        
        // Crear nombre único para el rectángulo
        string rect_name = "OB_" + ob.id;
        
        // Determinar color y estilo según tipo y estado
        color rect_color;
        ENUM_LINE_STYLE line_style;
        bool fill_rect = false;
        
        if(ob.tipo == 1) // Alcista
        {
            if(ob.operable && !ob.mitigado)
            {
                rect_color = clrDodgerBlue;
                line_style = STYLE_SOLID; // Línea continua para operables
            }
            else if(ob.mitigado)
            {
                rect_color = clrGray;
                line_style = STYLE_DASH; // Línea discontinua para mitigados
                fill_rect = false; // Solo contorno para mitigados
            }
            else
            {
                rect_color = clrLightBlue;
                line_style = STYLE_DOT;
            }
        }
        else // Bajista
        {
            if(ob.operable && !ob.mitigado)
            {
                rect_color = clrCrimson;
                line_style = STYLE_SOLID; // Línea continua para operables
            }
            else if(ob.mitigado)
            {
                rect_color = clrGray;
                line_style = STYLE_DASH; // Línea discontinua para mitigados
                fill_rect = false; // Solo contorno para mitigados
            }
            else
            {
                rect_color = clrLightPink;
                line_style = STYLE_DOT;
            }
        }
        
        // Calcular coordenadas
        datetime time_start = ob.time;
        datetime time_end;
        
        // Para OrderBlocks mitigados: no desplazar en el tiempo, solo marcar contorno
        if(ob.mitigado)
        {
            // Mantener el tamaño original de la vela del OrderBlock
            time_end = time_start + PeriodSeconds() * 1; // Solo la vela original
        }
        else
        {
            // Para OrderBlocks activos: proyectar hacia el futuro
            time_end = TimeCurrent() + PeriodSeconds() * 50;
        }
        
        double price_top = ob.high;
        double price_bottom = ob.low;
        
        // Crear rectángulo
        if(ObjectCreate(0, rect_name, OBJ_RECTANGLE, 0, time_start, price_top, time_end, price_bottom))
        {
            // Configurar propiedades del rectángulo
            ObjectSetInteger(0, rect_name, OBJPROP_COLOR, rect_color);
            ObjectSetInteger(0, rect_name, OBJPROP_STYLE, line_style);
            ObjectSetInteger(0, rect_name, OBJPROP_WIDTH, ob.operable && !ob.mitigado ? 2 : 1);
            ObjectSetInteger(0, rect_name, OBJPROP_FILL, fill_rect);
            ObjectSetInteger(0, rect_name, OBJPROP_BACK, true);
            ObjectSetInteger(0, rect_name, OBJPROP_SELECTABLE, false);
            ObjectSetInteger(0, rect_name, OBJPROP_SELECTED, false);
            
            // Crear tooltip con información
            string tooltip = "OrderBlock " + (ob.tipo == 1 ? "Alcista" : "Bajista") + "\n";
            tooltip += "ID: " + ob.id + "\n";
            tooltip += "Tamaño: " + DoubleToString(ob.tamano, _Digits) + "\n";
            tooltip += "Punto Medio: " + DoubleToString(ob.punto_medio, _Digits) + "\n";
            tooltip += "Estado: " + (ob.mitigado ? "Mitigado" : "Activo") + "\n";
            tooltip += "Operable: " + (ob.operable ? "Sí" : "No") + "\n";
            
            if(Confluencia) tooltip += "Confluencia: " + (ob.confluencia ? "Sí" : "No") + "\n";
            if(BreakofStructure) tooltip += "Break of Structure: " + (ob.breakofstructure ? "Sí" : "No") + "\n";
            if(LiqPrevOB) tooltip += "Liquidez Previa: " + (ob.liquidez_previa ? "Sí" : "No") + "\n";
            
            ObjectSetString(0, rect_name, OBJPROP_TOOLTIP, tooltip);
            
            if(debug)
            {
                Print("OrderBlock graficado: ", ob.id, " - Estado: ", (ob.mitigado ? "Mitigado" : "Activo"));
            }
        }
        else
        {
            if(debug) Print("Error creando rectángulo para OB: ", ob.id);
        }
    }
    
    if(debug) Print("Graficación de OrderBlocks completada. Total graficados: ", ArraySize(g_OrderBlocks));
}

//+------------------------------------------------------------------+
//| Función para colocar órdenes en OrderBlocks                     |
//+------------------------------------------------------------------+
void ColocarOrdenesOrderBlocks()
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    if(debug) Print("Colocando órdenes en OrderBlocks...");
    
    int totalOrderBlocks = ArraySize(g_OrderBlocks);
    double precioActual = SymbolInfoDouble(_Symbol, SYMBOL_BID);
    
    // Verificar si ya tenemos el máximo de trades por sesión
    if(g_TradesPorSesion >= MaxTradesPorSesion)
    {
        if(debug) Print("Máximo de trades por sesión alcanzado: ", g_TradesPorSesion);
        return;
    }
    
    for(int i = 0; i < totalOrderBlocks; i++)
    {
        // Verificar condiciones básicas
        if(!g_OrderBlocks[i].operable)
        {
            if(debug) 
            {
                Print("OrderBlock ", g_OrderBlocks[i].id, " no es operable. Razones:");
                Print("  - Mitigado: ", g_OrderBlocks[i].mitigado ? "SÍ" : "NO");
                Print("  - Ya operado: ", g_OrderBlocks[i].operado ? "SÍ" : "NO");
                Print("  - Punto medio: ", g_OrderBlocks[i].punto_medio);
                Print("  - Precio actual: ", precioActual);
                Print("  - Distancia al punto medio: ", MathAbs(precioActual - g_OrderBlocks[i].punto_medio) / _Point, " puntos");
            }
            continue;
        }
        
        if(g_OrderBlocks[i].mitigado)
        {
            if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " ya fue mitigado.");
            continue;
        }
        
        if(g_OrderBlocks[i].operado)
        {
            if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " ya fue operado.");
            continue;
        }
        
        ENUM_ORDER_TYPE tipoOrden = (g_OrderBlocks[i].tipo == 1) ? ORDER_TYPE_BUY_LIMIT : ORDER_TYPE_SELL_LIMIT;
        
        // Determinar estrategia de entrada según el tamaño del OrderBlock
        double tamanoOB = g_OrderBlocks[i].tamano / _Point; // Convertir a puntos
        
        // No operar OrderBlocks que excedan el tamaño máximo
        if(tamanoOB >= TamanoMaximoOB)
        {
            if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " descartado: tamaño excede máximo (", tamanoOB, " >= ", TamanoMaximoOB, ")");
            continue;
        }
        
        if(tamanoOB <= TamanoMinOB)
        {
            // OrderBlock pequeño: una sola entrada en el extremo
            double precioEntrada;
            string tipoEntrada;
            
            if(g_OrderBlocks[i].tipo == 1) // OrderBlock Alcista
            {
                precioEntrada = g_OrderBlocks[i].high;
                tipoEntrada = "High";
            }
            else // OrderBlock Bajista
            {
                precioEntrada = g_OrderBlocks[i].low;
                tipoEntrada = "Low";
            }
            
            if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " es pequeño (", tamanoOB, " puntos <= ", TamanoMinOB, "). Colocando entrada única en ", tipoEntrada, ": ", precioEntrada);
            
            string comentario = "OrderBlock " + g_OrderBlocks[i].id + "_" + tipoEntrada;
            
            // Verificar condiciones para entrada única
            if(MathAbs(precioActual - precioEntrada) > DistanciaMaximaAlPuntoEntrada * _Point)
            {
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " descartado: precio muy lejos del ", tipoEntrada, " (", MathAbs(precioActual - precioEntrada) / _Point, " puntos).");
                continue;
            }
            
            if(ExisteOrdenCercana(precioEntrada, DistanciaMinimaEntreOrdenes))
            {
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " descartado: orden cercana al ", tipoEntrada, ".");
                continue;
            }
            
            if(YaExisteOrdenPendiente(comentario))
            {
                if(debug) Print("Ya existe orden para OrderBlock ", g_OrderBlocks[i].id, " en ", tipoEntrada);
                continue;
            }
            
            // Verificar mitigación en tiempo real
            if(VerificarMitigacionOB(g_OrderBlocks[i]))
            {
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " está mitigado");
                g_OrderBlocks[i].mitigado = true;
                continue;
            }
            
            // Colocar orden única
            if(ColocarOrden(tipoOrden, precioEntrada, comentario))
            {
                g_OrderBlocks[i].operado = true;
                g_TradesPorSesion++;
                
                if(debug) Print("Orden única colocada para OrderBlock ", g_OrderBlocks[i].id, " en ", tipoEntrada, ": ", precioEntrada);
                
                // Verificar límite de trades por sesión
                if(g_TradesPorSesion >= MaxTradesPorSesion)
                {
                    if(debug) Print("Máximo de trades por sesión alcanzado después de colocar orden");
                    break;
                }
            }
            else
            {
                if(debug) Print("Error al colocar orden única para OrderBlock: ", g_OrderBlocks[i].id);
            }
        }
        else
        {
            // OrderBlock grande: una sola entrada en el punto medio
            if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " es grande (", tamanoOB, " puntos > ", TamanoMinOB, "). Colocando entrada única en punto medio.");
            
            double puntoMedio = g_OrderBlocks[i].punto_medio;
            string comentario = "OrderBlock " + g_OrderBlocks[i].id + "_PM";
            
            // Verificar condiciones para entrada única
            if(MathAbs(precioActual - puntoMedio) > DistanciaMaximaAlPuntoEntrada * _Point)
            {
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " descartado: precio muy lejos del punto medio (", MathAbs(precioActual - puntoMedio) / _Point, " puntos).");
                continue;
            }
            
            if(ExisteOrdenCercana(puntoMedio, DistanciaMinimaEntreOrdenes))
            {
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " descartado: orden cercana al punto medio.");
                continue;
            }
            
            if(YaExisteOrdenPendiente(comentario))
            {
                if(debug) Print("Ya existe orden para OrderBlock ", g_OrderBlocks[i].id, " en punto medio");
                continue;
            }
            
            // Verificar mitigación en tiempo real
            if(VerificarMitigacionOB(g_OrderBlocks[i]))
            {
                if(debug) Print("OrderBlock ", g_OrderBlocks[i].id, " está mitigado");
                g_OrderBlocks[i].mitigado = true;
                continue;
            }
            
            // Colocar orden única
            if(ColocarOrden(tipoOrden, puntoMedio, comentario))
            {
                g_OrderBlocks[i].operado = true;
                g_TradesPorSesion++;
                
                if(debug) Print("Orden única colocada para OrderBlock ", g_OrderBlocks[i].id, " en punto medio: ", puntoMedio);
                
                // Verificar límite de trades por sesión
                if(g_TradesPorSesion >= MaxTradesPorSesion)
                {
                    if(debug) Print("Máximo de trades por sesión alcanzado después de colocar orden");
                    break;
                }
            }
            else
            {
                if(debug) Print("Error al colocar orden única para OrderBlock: ", g_OrderBlocks[i].id);
            }
        }
        
        // Verificar límite global de trades por sesión
        if(g_TradesPorSesion >= MaxTradesPorSesion)
        {
            if(debug) Print("Máximo de trades por sesión alcanzado. Finalizando colocación de órdenes.");
            break;
        }
    }
    
    if(debug) Print("Colocación de órdenes en OrderBlocks completada. Total trades en sesión: ", g_TradesPorSesion);
}

//+------------------------------------------------------------------+
//| Función para verificar mitigación de OrderBlock                 |
//+------------------------------------------------------------------+
bool VerificarMitigacionOB(OrderBlock &ob)
{
    bool debug = g_DebugMode && DebugOrderBlocks;
    
    // Obtener precio actual
    double precio_actual = SymbolInfoDouble(_Symbol, SYMBOL_BID);
    
    // Para OB Alcista: mitigado si el precio alcanza el 50% del OB (punto medio)
    if(ob.tipo == 1)
    {
        bool mitigado = precio_actual <= ob.punto_medio;
        if(debug && mitigado) Print("OB Alcista ", ob.id, " mitigado. Precio: ", precio_actual, " <= Punto medio: ", ob.punto_medio);
        return mitigado;
    }
    // Para OB Bajista: mitigado si el precio alcanza el 50% del OB (punto medio)
    else if(ob.tipo == 0)
    {
        bool mitigado = precio_actual >= ob.punto_medio;
        if(debug && mitigado) Print("OB Bajista ", ob.id, " mitigado. Precio: ", precio_actual, " >= Punto medio: ", ob.punto_medio);
        return mitigado;
    }
    
    return false;
}

void OperarLiquidezPrevia()
{
    bool debug = g_DebugMode;
    if(debug) Print("Analizando liquidez previa...");
    
    // Aquí iría la lógica para detectar, verificar, graficar y operar liquidez previa
    // Esta función se completará en futuras actualizaciones
}

//+------------------------------------------------------------------+
//| Función para calcular lotaje con riesgo manual por trade         |
//+------------------------------------------------------------------+
double CalcularLotaje()
{
    bool debug = g_DebugMode && DebugLotaje;
    if(debug) Print("Calculando lotaje con riesgo manual por trade...");
    
    // Obtener el balance actual
    double balanceActual = AccountInfoDouble(ACCOUNT_BALANCE);
    
    // Usar directamente el riesgo por trade configurado
    double riesgoCalculado = RiesgoPorTrade;
    
    if(debug) Print("Usando riesgo manual por trade: ", riesgoCalculado, "%");
    
    // Calcular el lotaje según la fórmula: (Balance * Riesgo por trade) / (Stop Loss en pips * 10)
    // StopLossPoints está en puntos, se convierte a pips dividiendo entre 10
    double montoRiesgo = (balanceActual * riesgoCalculado / 100.0);
    double stopLossEnPips = StopLossPoints / 10.0;
    double lotaje = montoRiesgo / (stopLossEnPips * 10.0);
    
    // Ajustar el lotaje al mínimo permitido por el bróker
    double lotajeMinimo = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_MIN);
    if(lotaje < lotajeMinimo)
        lotaje = lotajeMinimo;
    
    // Ajustar el lotaje al máximo permitido por el bróker
    double lotajeMaximo = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_MAX);
    if(lotaje > lotajeMaximo)
        lotaje = lotajeMaximo;
    
    // Redondear al paso de lote permitido
    double lotajeStep = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_STEP);
    lotaje = MathFloor(lotaje / lotajeStep) * lotajeStep;
    
    if(debug) Print("Lotaje calculado: ", lotaje, " (Riesgo: ", riesgoCalculado, "%, Monto: ", montoRiesgo, ", Balance: ", balanceActual, ")");
    
    return lotaje;
}

//+------------------------------------------------------------------+
//| Función para calcular lotaje con sistema automático (legacy)     |
//+------------------------------------------------------------------+
double CalculoDelLotaje()
{
    bool debug = g_DebugMode && DebugLotaje;
    if(debug) Print("Calculando tamaño del lote...");
    
    // Obtener el balance actual
    double balanceActual = AccountInfoDouble(ACCOUNT_BALANCE);
    
    // Si es la primera vez que se ejecuta, guardar el balance inicial
    if(g_BalanceInicial <= 0)
    {
        // Intentar obtener el balance inicial histórico (esto dependerá de la implementación del bróker)
        // En este ejemplo, simplemente usamos el balance actual como inicial
        g_BalanceInicial = balanceActual;
        if(debug) Print("Balance inicial establecido: ", g_BalanceInicial);
    }
    
    double riesgoCalculado = 0.0;
    double lotaje = 0.0;
    
    // Calcular el porcentaje de cambio del balance
    double porcentajeCambio = ((balanceActual - g_BalanceInicial) / g_BalanceInicial) * 100.0;
    
    if(debug) Print("Balance inicial: ", g_BalanceInicial, ", Balance actual: ", balanceActual, ", Cambio: ", porcentajeCambio, "%");
    
    // Calcular el riesgo según la configuración
    if(OperarRiesgoManual)
    {
        // Usar el riesgo por trade configurado directamente
        riesgoCalculado = RiesgoPorTrade;
        if(debug) Print("Usando riesgo manual por trade: ", riesgoCalculado, "%");
    }
    else if(OperarRiesgoautomático)
    {
        // Calcular el riesgo automático según el estado de la cuenta
        if(Cuentafondeo) // Reglas para cuenta de fondeo
        {
            if(porcentajeCambio >= -3.0 && porcentajeCambio <= 3.0)
                riesgoCalculado = 0.33;
            else if(porcentajeCambio > 3.0 && porcentajeCambio <= 5.0)
                riesgoCalculado = 0.66;
            else if(porcentajeCambio > 5.0)
                riesgoCalculado = 0.99;
            else if(porcentajeCambio >= -5.0 && porcentajeCambio < -3.0)
                riesgoCalculado = 0.25;
            else if(porcentajeCambio >= -7.0 && porcentajeCambio < -5.0)
                riesgoCalculado = 0.15;
            else if(porcentajeCambio >= -9.0 && porcentajeCambio < -7.0)
                riesgoCalculado = 0.10;
            else if(porcentajeCambio >= -9.5 && porcentajeCambio < -9.0)
                riesgoCalculado = 0.03;
            else if(porcentajeCambio >= -10.0 && porcentajeCambio < -9.5)
                riesgoCalculado = 0.01;
            else
                riesgoCalculado = 0.01; // Valor mínimo por defecto
        }
        else // Reglas para cuenta normal
        {
            if(porcentajeCambio >= -6.0 && porcentajeCambio <= 6.0)
                riesgoCalculado = 0.66;
            else if(porcentajeCambio > 6.0 && porcentajeCambio <= 10.0)
                riesgoCalculado = 1.2;
            else if(porcentajeCambio > 10.0)
                riesgoCalculado = 2.0;
            else if(porcentajeCambio >= -10.0 && porcentajeCambio < -6.0)
                riesgoCalculado = 0.50;
            else if(porcentajeCambio >= -14.0 && porcentajeCambio < -10.0)
                riesgoCalculado = 0.33;
            else if(porcentajeCambio >= -16.0 && porcentajeCambio < -14.0)
                riesgoCalculado = 0.25;
            else if(porcentajeCambio >= -18.0 && porcentajeCambio < -16.0)
                riesgoCalculado = 0.15;
            else if(porcentajeCambio >= -20.0 && porcentajeCambio < -18.0)
                riesgoCalculado = 0.10;
            else
                riesgoCalculado = 0.10; // Valor mínimo por defecto
        }
        
        if(debug) Print("Riesgo automático calculado: ", riesgoCalculado, "% (Cambio balance: ", porcentajeCambio, "%)");
    }
    else
    {
        // Si no hay configuración de riesgo, usar un valor por defecto
        riesgoCalculado = 0.5;
        if(debug) Print("No hay configuración de riesgo definida, usando valor por defecto: ", riesgoCalculado, "%");
    }
    
    // Guardar el último riesgo calculado
    g_UltimoRiesgoCalculado = riesgoCalculado;
    
    // Calcular el lotaje según la fórmula: (Tamaño cuenta * Riesgo por trade)/(stop loss (pips) * 10)
    // StopLossPoints está en puntos, se convierte a pips dividiendo entre 10
    double montoRiesgo = (balanceActual * riesgoCalculado / 100.0);
    double stopLossEnPips = StopLossPoints / 10.0;
    lotaje = montoRiesgo / (stopLossEnPips * 10.0);
    
    // Ajustar el lotaje al mínimo permitido por el bróker
    double lotajeMinimo = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_MIN);
    if(lotaje < lotajeMinimo)
        lotaje = lotajeMinimo;
    
    // Ajustar el lotaje al máximo permitido por el bróker
    double lotajeMaximo = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_MAX);
    if(lotaje > lotajeMaximo)
        lotaje = lotajeMaximo;
    
    // Redondear al paso de lote permitido
    double lotajeStep = SymbolInfoDouble(_Symbol, SYMBOL_VOLUME_STEP);
    lotaje = MathFloor(lotaje / lotajeStep) * lotajeStep;
    
    if(debug) Print("Lotaje calculado: ", lotaje, " (Riesgo: ", riesgoCalculado, "%, Monto: ", montoRiesgo, ")");
    
    return lotaje;
}

//+------------------------------------------------------------------+
//| Función para aplicar break even a órdenes abiertas               |
//+------------------------------------------------------------------+
void AplicarBreakEven()
{
    bool debug = g_DebugMode && DebugBreakEven;
    if(debug) Print("Verificando condiciones para break even...");
    
    // Obtener todas las órdenes abiertas
    int totalPosiciones = PositionsTotal();
    
    for(int i = 0; i < totalPosiciones; i++)
    {
        ulong ticket = PositionGetTicket(i);
        if(ticket <= 0) continue;
        
        // Verificar si la posición pertenece al símbolo actual
        if(PositionGetString(POSITION_SYMBOL) != _Symbol) continue;
        
        // Obtener información de la posición
        double precioApertura = PositionGetDouble(POSITION_PRICE_OPEN);
        double stopLoss = PositionGetDouble(POSITION_SL);
        double takeProfit = PositionGetDouble(POSITION_TP);
        int tipo = (PositionGetInteger(POSITION_TYPE) == POSITION_TYPE_BUY) ? 1 : 0; // 1 = compra, 0 = venta
        
        // Obtener precio actual
        double precioActual = (tipo == 1) ? SymbolInfoDouble(_Symbol, SYMBOL_BID) : SymbolInfoDouble(_Symbol, SYMBOL_ASK);
        
        // Calcular la distancia en puntos entre el precio de apertura y el stop loss
        double distanciaSL = MathAbs(precioApertura - stopLoss) / _Point;
        
        // Calcular el nivel de break even inicial (cuando el precio alcanza StopLossPoints*BreakevenRatio)
        double nivelBreakEvenInicial = distanciaSL * BreakevenRatio;
        
        // Calcular la distancia actual del precio al precio de apertura
        double distanciaActual = 0;
        if(tipo == 1) // Compra
            distanciaActual = (precioActual - precioApertura) / _Point;
        else // Venta
            distanciaActual = (precioApertura - precioActual) / _Point;
        
        // Buscar si ya tenemos esta posición en nuestro array
        int posicionIndex = -1;
        for(int j = 0; j < ArraySize(g_Ordenes); j++)
        {
            if(g_Ordenes[j].ticket == ticket)
            {
                posicionIndex = j;
                break;
            }
        }
        
        // Si no encontramos la orden, añadirla al array
        if(posicionIndex == -1)
        {
            int size = ArraySize(g_Ordenes);
            ArrayResize(g_Ordenes, size + 1);
            g_Ordenes[size].ticket = ticket;
            g_Ordenes[size].precioApertura = precioApertura;
            g_Ordenes[size].stopLoss = stopLoss;
            g_Ordenes[size].takeProfit = takeProfit;
            g_Ordenes[size].tipo = tipo;
            g_Ordenes[size].breakEvenAplicado = false;
            g_Ordenes[size].nivelBreakEven = 0;
            posicionIndex = size;
        }
        
        if(!g_Ordenes[posicionIndex].breakEvenAplicado && distanciaActual >= nivelBreakEvenInicial)
        {
            // Calcular nuevo stop loss (precio de apertura +/- 10 puntos)
            double nuevoSL = 0;
            if(tipo == 1) // Compra
                nuevoSL = precioApertura + 10 * _Point;
            else // Venta
                nuevoSL = precioApertura - 10 * _Point;
            
            // Modificar la posición
            bool modificado = trade.PositionModify(ticket, nuevoSL, takeProfit);
            
            if(modificado)
            {
                if(debug) Print("Break even aplicado a posición #", ticket, ". Nuevo SL: ", nuevoSL);
                g_Ordenes[posicionIndex].breakEvenAplicado = true;
                g_Ordenes[posicionIndex].nivelBreakEven = 1;
                g_Ordenes[posicionIndex].stopLoss = nuevoSL;
            }
            else
            {
                if(debug) Print("Error al aplicar break even a posición #", ticket, ". Error: ", GetLastError());
            }
        }
        // Verificar si debemos ajustar el stop loss a niveles adicionales de beneficio
        else if(g_Ordenes[posicionIndex].breakEvenAplicado)
        {
            // Calcular el siguiente nivel de ajuste (cada vez que alcanza un nuevo ratio de beneficio)
            int nivelActual = g_Ordenes[posicionIndex].nivelBreakEven;
            double siguienteNivel = nivelBreakEvenInicial * (nivelActual + 1);
            
            if(distanciaActual >= siguienteNivel)
            {
                // Calcular nuevo stop loss (mover a favor StopLossPoints * 0.5 * nivel)
                double nuevoSL = 0;
                if(tipo == 1) // Compra
                    nuevoSL = g_Ordenes[posicionIndex].stopLoss + (StopLossPoints * 0.5 * _Point);
                else // Venta
                    nuevoSL = g_Ordenes[posicionIndex].stopLoss - (StopLossPoints * 0.5 * _Point);
                
                // Modificar la orden
                bool modificado = trade.PositionModify(ticket, nuevoSL, takeProfit);
                
                if(modificado)
                {
                    if(debug) Print("Ajuste adicional de SL aplicado a orden #", ticket, ". Nuevo SL: ", nuevoSL, " (Nivel ", nivelActual + 1, ")");
                    g_Ordenes[posicionIndex].nivelBreakEven++;
                    g_Ordenes[posicionIndex].stopLoss = nuevoSL;
                }
                else
                {
                    if(debug) Print("Error al ajustar SL adicional a orden #", ticket, ". Error: ", GetLastError());
                }
            }
        }
    }
}

bool ColocarOrden(ENUM_ORDER_TYPE tipoOrden, double precioEntrada, string comentario = "")
{
    bool debug = g_DebugMode && DebugColocarOrden;
    if(debug) Print("Intentando colocar orden: ", EnumToString(tipoOrden), " a precio: ", precioEntrada);
    
    // Verificar que el tipo de orden sea válido (solo limit)
    if(tipoOrden != ORDER_TYPE_BUY_LIMIT && tipoOrden != ORDER_TYPE_SELL_LIMIT)
    {
        if(debug) Print("Tipo de orden no soportado: ", EnumToString(tipoOrden));
        return false;
    }
    
    // Obtener el precio actual
    double precioActual = 0;
    if(tipoOrden == ORDER_TYPE_BUY_LIMIT)
        precioActual = SymbolInfoDouble(_Symbol, SYMBOL_ASK);
    else
        precioActual = SymbolInfoDouble(_Symbol, SYMBOL_BID);
    
    // Verificar si el precio está lo suficientemente cerca para colocar la orden
    double distancia = MathAbs(precioActual - precioEntrada);
    double puntosSimbolo = SymbolInfoDouble(_Symbol, SYMBOL_POINT);
    double distanciaMaxima = 100 * puntosSimbolo; // 100 puntos de distancia máxima (ajustable)
    
    if(distancia > distanciaMaxima)
    {
        if(debug) Print("Precio demasiado lejos para colocar orden. Distancia: ", distancia / puntosSimbolo, " puntos");
        return false;
    }
    
    // Calcular el tamaño del lote
    double lotaje = CalculoDelLotaje();
    
    // Calcular StopLoss y TakeProfit
    double stopLoss = 0, takeProfit = 0;
    
    if(tipoOrden == ORDER_TYPE_BUY_LIMIT)
    {
        stopLoss = precioEntrada - (StopLossPoints * SymbolInfoDouble(_Symbol, SYMBOL_POINT));
        takeProfit = precioEntrada + (StopLossPoints * RiesgoRecompensa * SymbolInfoDouble(_Symbol, SYMBOL_POINT));
    }
    else // ORDER_TYPE_SELL_LIMIT
    {
        stopLoss = precioEntrada + (StopLossPoints * SymbolInfoDouble(_Symbol, SYMBOL_POINT));
        takeProfit = precioEntrada - (StopLossPoints * RiesgoRecompensa * SymbolInfoDouble(_Symbol, SYMBOL_POINT));
    }
    
    // Normalizar los precios según los ticks mínimos
    double tickSize = SymbolInfoDouble(_Symbol, SYMBOL_TRADE_TICK_SIZE);
    precioEntrada = NormalizeDouble(precioEntrada, _Digits);
    stopLoss = NormalizeDouble(stopLoss, _Digits);
    takeProfit = NormalizeDouble(takeProfit, _Digits);
    
    // Colocar la orden
    if(debug) Print("Colocando orden: ", EnumToString(tipoOrden), ", Precio: ", precioEntrada, ", SL: ", stopLoss, ", TP: ", takeProfit, ", Lote: ", lotaje);
    
    // Usar el objeto trade para colocar la orden
    bool resultado = false;
    
    if(tipoOrden == ORDER_TYPE_BUY_LIMIT)
    {
        resultado = trade.BuyLimit(lotaje, precioEntrada, _Symbol, stopLoss, takeProfit, ORDER_TIME_GTC, 0, comentario);
    }
    else // ORDER_TYPE_SELL_LIMIT
    {
        resultado = trade.SellLimit(lotaje, precioEntrada, _Symbol, stopLoss, takeProfit, ORDER_TIME_GTC, 0, comentario);
    }
    
    // Verificar el resultado
    if(resultado)
    {
        if(debug) Print("Orden colocada exitosamente. Ticket: ", trade.ResultOrder());
        
        // Incrementar el contador de trades por sesión
        g_TradesPorSesion++;
        
        // Guardar información de la orden para el break even y detección de falsos positivos
        int size = ArraySize(g_Ordenes);
        ArrayResize(g_Ordenes, size + 1);
        g_Ordenes[size].ticket = trade.ResultOrder();
        g_Ordenes[size].precioApertura = precioEntrada;
        g_Ordenes[size].stopLoss = stopLoss;
        g_Ordenes[size].takeProfit = takeProfit;
        g_Ordenes[size].tipo = (tipoOrden == ORDER_TYPE_BUY_LIMIT) ? 1 : 0;
        g_Ordenes[size].breakEvenAplicado = false;
        g_Ordenes[size].nivelBreakEven = 0;
        // Inicializar campos para detección de falsos positivos
        g_Ordenes[size].distanciaMinima = DBL_MAX; // Inicializar con un valor alto
        g_Ordenes[size].marcadoParaEliminacion = false;
        g_Ordenes[size].tiempoColocacion = TimeCurrent();
        
        return true;
    }
    else
    {
        if(debug) Print("Error al colocar la orden: ", GetLastError(), ", ", trade.ResultRetcode(), ", ", trade.ResultRetcodeDescription());
        return false;
    }
}

//+------------------------------------------------------------------+
//| Función para detectar y eliminar falsos positivos               |
//+------------------------------------------------------------------+
void DetectarFalsosPositivos()
{
    bool debug = g_DebugMode && DebugColocarOrden;
    if(debug) Print("Verificando falsos positivos en órdenes pendientes...");
    
    // Obtener precio actual
    double precioActual = SymbolInfoDouble(_Symbol, SYMBOL_BID);
    
    // Revisar todas las órdenes pendientes
    for(int i = OrdersTotal() - 1; i >= 0; i--)
    {
        ulong ticket = OrderGetTicket(i);
        if(ticket <= 0) continue;
        
        // Verificar si la orden pertenece al símbolo actual
        if(OrderGetString(ORDER_SYMBOL) != _Symbol) continue;
        
        // Obtener información de la orden
        ENUM_ORDER_TYPE tipoOrden = (ENUM_ORDER_TYPE)OrderGetInteger(ORDER_TYPE);
        double precioEntrada = OrderGetDouble(ORDER_PRICE_OPEN);
        double stopLoss = OrderGetDouble(ORDER_SL);
        
        // Solo procesar órdenes limit
        if(tipoOrden != ORDER_TYPE_BUY_LIMIT && tipoOrden != ORDER_TYPE_SELL_LIMIT)
            continue;
        
        // Buscar la orden en nuestro array
        int ordenIndex = -1;
        for(int j = 0; j < ArraySize(g_Ordenes); j++)
        {
            if(g_Ordenes[j].ticket == ticket)
            {
                ordenIndex = j;
                break;
            }
        }
        
        // Si no encontramos la orden en nuestro array, saltarla
        if(ordenIndex == -1) continue;
        
        // Calcular distancia actual al precio de entrada
        double distanciaActual = MathAbs(precioActual - precioEntrada);
        
        // Actualizar distancia mínima si es menor
        if(distanciaActual < g_Ordenes[ordenIndex].distanciaMinima)
        {
            g_Ordenes[ordenIndex].distanciaMinima = distanciaActual;
        }
        
        // Calcular distancia del stop loss
        double distanciaSL = MathAbs(precioEntrada - stopLoss);
        double umbralEliminacion = distanciaSL * 2.0; // 2 veces el stop loss
        
        // Verificar condiciones para falso positivo
        bool seAproximo = g_Ordenes[ordenIndex].distanciaMinima <= (distanciaSL * 0.5); // Se aproximó a la mitad del SL
        bool seAlejo = distanciaActual >= umbralEliminacion; // Se alejó 2 veces el SL
        
        if(seAproximo && seAlejo && !g_Ordenes[ordenIndex].marcadoParaEliminacion)
        {
            // Marcar como falso positivo y eliminar la orden
            g_Ordenes[ordenIndex].marcadoParaEliminacion = true;
            
            if(debug) Print("Falso positivo detectado en orden #", ticket, 
                          ". Distancia mínima: ", g_Ordenes[ordenIndex].distanciaMinima/_Point, " puntos",
                          ", Distancia actual: ", distanciaActual/_Point, " puntos",
                          ", Umbral eliminación: ", umbralEliminacion/_Point, " puntos");
            
            // Eliminar la orden
            if(trade.OrderDelete(ticket))
            {
                if(debug) Print("Orden #", ticket, " eliminada por falso positivo");
                
                // Remover del array de órdenes
                for(int k = ordenIndex; k < ArraySize(g_Ordenes) - 1; k++)
                {
                    g_Ordenes[k] = g_Ordenes[k + 1];
                }
                ArrayResize(g_Ordenes, ArraySize(g_Ordenes) - 1);
            }
            else
            {
                if(debug) Print("Error al eliminar orden #", ticket, ": ", GetLastError());
            }
        }
    }
}

void CerrarOperacionesAntesCierreNY()
{
    // Obtener hora actual
    MqlDateTime tiempo_actual;
    TimeToStruct(TimeCurrent(), tiempo_actual);
    
    int hora_actual = tiempo_actual.hour;
    int minuto_actual = tiempo_actual.min;
    
    // Ajustar por GMT Offset
    hora_actual = (hora_actual + GMTOffset) % 24;
    if(hora_actual < 0) hora_actual += 24;
    
    // Verificar si estamos 15 minutos antes del cierre de NY
    bool es_tiempo_cierre = false;
    
    // Si es la hora de cierre menos 1 y los minutos son >= 45, o es exactamente la hora de cierre y minutos = 0
    if((hora_actual == HoraCierreNY - 1 && minuto_actual >= 45) || 
       (hora_actual == HoraCierreNY && minuto_actual == 0))
    {
        es_tiempo_cierre = true;
    }
    
    // Si es tiempo de cierre, cerrar todas las órdenes pendientes y posiciones abiertas
    if(es_tiempo_cierre)
    {
        bool debug = g_DebugMode;
        if(debug) Print("Cerrando todas las operaciones 15 minutos antes del cierre de NY...");
        
        // 1. Cerrar órdenes pendientes
        for(int i = OrdersTotal() - 1; i >= 0; i--)
        {
            ulong ticket = OrderGetTicket(i);
            if(ticket > 0)
            {
                string symbol = OrderGetString(ORDER_SYMBOL);
                if(symbol == _Symbol)
                {
                    trade.OrderDelete(ticket);
                    if(debug) Print("Orden pendiente #", ticket, " eliminada antes del cierre de NY");
                }
            }
        }
        
        // 2. Cerrar posiciones abiertas
        for(int i = PositionsTotal() - 1; i >= 0; i--)
        {
            ulong ticket = PositionGetTicket(i);
            if(ticket > 0)
            {
                string symbol = PositionGetString(POSITION_SYMBOL);
                if(symbol == _Symbol)
                {
                    trade.PositionClose(ticket);
                    if(debug) Print("Posición #", ticket, " cerrada antes del cierre de NY");
                }
            }
        }
    }
}
