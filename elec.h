extern void Uinit();

extern void Uend();

extern int sendenccmd(int botID, char action, int value=0, unsigned char speed=0);

extern void e_sendenccmd(int botID, char action, int value=0, unsigned char speed=0);

extern bool check_bot_free(int botID);

extern void ensure_bot_free(int botID);

extern void wait_4_bot(int botID);

extern void bot_status();

extern char bot_code[5][11];

