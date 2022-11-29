#ifndef ProcessBlock
#define ProcessBlock
class PCB{
   private:
      int JID;
      int TTL;
      int TLL;
      int TTC;
      int LLC;
   public:
      PCB(){
        TTC =0;
        LLC=0;
      }
      void setJID(int val){
          JID = val;
      }
      int getJID(){
         return JID;
      }
      void setTTL(int val){
           TTL = val;
      }
      int getTTL(){
         return TTL;
      }
      void setTLL(int val){
           TLL = val;
      }
      int getTLL(){
          return TLL;
      }
      void incrementTTC(){
        TTC++;
      }
      void incrementLLC(){
        LLC++;
      }
      bool isTTCExceed(){
          if(TTC<=TTL)
             return false;
        return true;
      }
      bool isLLCExceed(){
          if(LLC<=TLL)
             return false;
        return true;
      }
      void setTTC(int val){
          TTC=val;
      }
      int getTTC(){
          return TTC;
      }
      void setLLC(int val){
         LLC=val;
      }
      int getLLC(){
          return LLC;
      }
};
#endif //
