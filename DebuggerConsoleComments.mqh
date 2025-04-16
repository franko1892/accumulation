//+------------------------------------------------------------------+
#include <Arrays/ArrayString.mqh>

//+------------------------------------------------------------------+
class DebuggerConsoleComments
  {
private:
   static DebuggerConsoleComments *instance; // Static instance of the class.

   CArrayString            messages;

   // Private constructor to prevent instantiation from outside.
                     DebuggerConsoleComments() {}

   // Delete copy constructor and assignment operator to avoid duplication.
                     DebuggerConsoleComments(const DebuggerConsoleComments&) = delete;


public:
   // Static method to access the single instance of the class.
   static DebuggerConsoleComments* GetInstance()
     {
      if(instance == NULL)
        {
         instance = new DebuggerConsoleComments();
        }
      return instance;
     }

   // Public method for demonstration.
   void              Log(const string message)
     {
      string m = __FILE__+"->"+__FUNCSIG__+"->"+TimeToString(TimeTradeServer(), TIME_DATE | TIME_MINUTES | TIME_SECONDS)+":"+message;

      this.messages.Add(m);
      printf("Log: %s\n", message);
     }

   bool              Save(string filename)
     {
      int file_handle = FileOpen(filename, FILE_WRITE | FILE_TXT | FILE_UNICODE);
      int result = this.messages.Save(file_handle);
      FileClose(file_handle);
      return result > 0;
     }
  };

// Initialize the static member variable.
DebuggerConsoleComments* DebuggerConsoleComments::instance = NULL;
//+------------------------------------------------------------------+
