
float valorCorte[4] = {0, 0, 0, 0};
float valorPreto[4] = {0, 0, 0, 0};
float valorBranco[4] = {0, 0, 0, 0};

bool btnSubirRampa;
bool btnCalibrarBranco;
bool btnSeguirLinha;
bool seguirLinha = false;
bool rampa = false;

void curvaEsqSimples();
void curvaDirSimples();
void curvaEsq90();
void curvaDir90();

void calibrarBranco();
void calibrarPreto();
void initRampa();
void initSeguirLinha();
