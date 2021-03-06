#include "interface.h"

void interface( Mips& mips )
{
    char answer;
    string args[10];

    bool memview = false;
    bool regview = false;
    bool autoclear = true;

    backtobegin:

    if( autoclear )
    {
        system("cls");
        system("clear");
    }

    cout << "==== INTERFACE ====" << endl << endl;
    cout << "==> Recursos Visuais" << endl;
    cout << "r - Mostrar banco de registradores" << " [" << ( regview ? "sim":"não" ) << "]" << endl;
    cout << "m - Mostrar memória de dados" << " [" << ( memview ? "sim":"não" ) << "]" << endl;
    cout << "c - Autolimpar a tela" << " [" << ( autoclear ? "sim":"não" ) << "]" << endl;
    cout << "h - Mostrar o MENU novamente" << endl;
    cout << endl << "==> Ações" << endl;
    cout << "i - Inserir instrução MIPs ( binario )" << endl;
    cout << "l - Ligar simulador" << endl;
    cout << "p - Executar um passo da instrução" << endl;
    cout << "s - Fechar o programa" << endl;

    if( regview ) mips.PrintReg();
    if( memview ) mips.PrintMem();

    backtoinvalid:

    cin >> answer;

    switch( answer )
    {
        case 'r':
            regview = !regview;
        break;

        case 'm':
            memview = !memview;
        break;

        case 'c':
            autoclear = !autoclear;
        break;

        case 'i':
            cin >> args[0];
            mips.Execute( args[0] );
        break;

        case 'l':
            mips.Run();
        break;

        case 'p':
            mips.Step();
        break;

        case 'h':
            goto backtobegin;
        break;

        case 's':
            goto theend;
        break;

        default:
            goto backtoinvalid;
        break;
    }

    goto backtobegin;

    theend:

    return;
}