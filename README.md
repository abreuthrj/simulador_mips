# Compilado com
g++ *.cpp -I .\ -o mips.exe -finput-charset=utf-8

# Parametros para funcionamento
mips <arquivo_de_entrada>
# ou apenas mips caso possua entrada.txt no diretorio

# Auxiliar de função
mips genbin <comando_para_interpretacao>
# imprime no cout o comando traduzido em binário
# Ex:
# mips genbin addi $s0 $zero 10