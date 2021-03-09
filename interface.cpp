#include "interface.h"

void interface( Mips& mips )
{
    char answer;
    string args[10];

    bool memview = false;
    bool regview = false;
    bool autoclear = false;

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
    cout << "i - Inserir instrução MIPS ( binario )" << endl;
    cout << "q - Inserir instrução MIPS ( comando )" << endl;
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

        case 'q':
            cin >> args[0];
            if( args[0].compare("add") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000100000";
            }
            else if( args[0].compare("sub") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000100010";
            }
            else if( args[0].compare("and") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000100100";
            }
            else if( args[0].compare("or") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000100101";
            }
            else if( args[0].compare("slt") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000101010";
            }
            else if( args[0].compare("sll") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000000000";
            }
            else if( args[0].compare("addi") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "001000"+rtobin(args[2])+rtobin(args[1])+signedtobin( strtol( args[3].c_str(),nullptr,10 ), 16 );
            }
            else if( args[0].compare("lw") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "100011"+rtobin(args[3])+rtobin(args[1])+signedtobin( strtol( args[2].c_str(),nullptr,10 ), 16 );
            }
            else if( args[0].compare("sw") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "101011"+rtobin(args[3])+rtobin(args[1])+signedtobin( strtol( args[2].c_str(),nullptr,10 ), 16 );
            }
            else if( args[0].compare("beq") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000100"+rtobin(args[1])+rtobin(args[2])+dectobin( strtol( args[3].c_str(),nullptr,10 ), 16 );
            }
            else if( args[0].compare("bne") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000101"+rtobin(args[1])+rtobin(args[2])+dectobin( strtol( args[3].c_str(),nullptr,10 ), 16 );
            }
            else if( args[0].compare("j") == 0 )
            {
                cin >> args[1];
                args[4] = "000010"+dectobin( strtol( args[1].c_str(),nullptr,10 ), 26 );
            }
            else if( args[0].compare("jr") == 0 )
            {
                cin >> args[1] >> args[2] >> args[3];
                args[4] = "000101"+rtobin(args[1])+rtobin(args[2])+dectobin( strtol( args[3].c_str(),nullptr,10 ), 16 );
            }
            else if( args[0].compare("jal") == 0 )
            {
                args[4] = "000000"+rtobin(args[2])+rtobin(args[3])+rtobin(args[1])+"00000100000";
            }
            mips.Execute( args[4] );
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

string rtobin( string r )
{
    return ( 
             r.compare("$sp") == 0 ? dectobin( 29,5 ):
             r.compare("$zero") == 0 ? dectobin( 0,5 ):
             r.substr(0,2).compare("$s") == 0 ? dectobin( 16+(r[2]-'0'),5 ):
             r.substr(0,2).compare("$t") == 0 ? dectobin( 8+(r[2]-'0'),5 ):
             dectobin( 0,5 ) 
           );
}