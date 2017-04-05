module CallSystem {
    interface UserManager{
        int darAlta(int dni);
        int comprarMinutos(int dni,int minutos);
        int avisarConsumo(int dni,int minutesAlertThreshold);        
    };
};
