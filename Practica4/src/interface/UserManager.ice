module CallSystem {
    interface UserManager{
        int darAlta(int dni);
        int comprarMinutos(int dni,int minutos);
        int avisarConsumo(int dni,int minutesAlertThreshold,string myip);
        int connect(string myip,string port);
        int disconnect(string myip);
    };
};
