#! /bin/bash

if [[ -z ${MDM_BASEDIR} ]]; then
    echo "Variável MDM_BASEDIR não definida!"
    echo "Defina a variavel com o diretorio base do projeto mdm."
    exit 1
fi

INCLUDE=$MDM_BASEDIR/usr/local/share/mdm/include
ETC=$MDM_BASEDIR/etc
MDM=$ETC/mdm
INIT=$ETC/init.d

if [[ $# -ne 1 ]]; then
    echo "Uso: mdm_translate <arquivo.pot>"
    exit 0
fi

cat $INCLUDE/poHeader.c3sl > $1.pot

for file in `ls $INCLUDE/*.inc` $MDM/create_xorg_conf $MDM/mdm
do
    bash --dump-po-strings $file >> $1.pot
done

