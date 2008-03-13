#
# Funções de LOG do sistema
#
# Copyright (C) 2004-2008 Centro de Computacao Cientifica e Software Livre
# Departamento de Informatica - Universidade Federal do Parana - C3SL/UFPR#
#
# This is free software.  You may redistribute copies of it under the terms of
# the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.
# There is NO WARRANTY, to the extent permitted by law.

#**********************************
#
#  File Version: 1.0.0
#                30/01/2008
#  Dependencies:
#       none


#
# Esta função é responsável por escrever um log de debug do sistema.
# Usa a variável de ambiente $DEBUG_FILE como nome do arquivo de
# log a ser gravado.
#
# Parâmetros:   texto a ser gravado (obrigatório)
#               texto2 a ser gravado (opcional)
#

function WriteDebug()
{
    if [[ ! -z "${DEBUG_ENABLED}" &&  "${DEBUG_ENABLED}" -eq 1 ]]; then
        # cria o arquivo de log, caso não exista ainda
        local DBG=${DEBUG}/${DEBUG_FILE}
        
        # escreve o log no arquivo de debug
        echo -e "`date "+%D %R"` $1 $2" >> ${DBG}
        
        # escreve o log na saida de erro padrão
        echo -e "$1" "$2" >&2
    fi
}

#
# Esta função escreve um log genérico do sistema dentro do arquivo
# mdm_sys.log
#
# Parâmetros:   texto a ser gravado (obrigatório)
#               texto2 a ser gravado (opcional)
#

function WriteLog()
{
    local LOG=${DEBUG}/mdm-sys.log
    
    # escreve o log no arquivo de logs
    echo -e "`date "+%D %R"` $1 $2" >> ${LOG}
}
