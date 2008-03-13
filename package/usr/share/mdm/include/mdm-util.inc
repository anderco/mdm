#
#  Utilitários do sistema mdm 
#
# Copyright (C) 2004-2008 Centro de Computacao Cientifica e Software Livre
# Departamento de Informatica - Universidade Federal do Parana - C3SL/UFPR
#
# This is free software.  You may redistribute copies of it under the terms of
# the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.
# There is NO WARRANTY, to the extent permitted by law.

#**********************************
#
#  File Version: 1.0.0
#                30/01/2008
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc

#
# Esta funcão verifica se o usuário é o root.
#
# Parâmetros:   nenhum
# 
# Saida:        escreve na saida padrão:
#                   0   usuário comum
#                   1   usuário é root
#

function IsRoot()
{
    if [[ `id -u` -eq 0 ]]; then
         echo 1
    else
        echo 0
    fi
} # IsRoot

#
# Esta função verifica se estão instalados os programas necessários
# para o perfeito funcionamento do mdm.
#
# Parâmetros:   nenhum
#

function CheckSystem()
{
    # Seta o nome do arquivo de log a ser gravado
    local DEBUG_FILE=System.log
    
    # Verifica se o Discover está instalado
    if [[ -z "${Discover}" ]]; then
        WriteDebug $"Detecção Drivers de Video desabilitada." \
                   $"Discover não encontrado."
    fi
    
    # Verifica se o GDMDynamic está instalado
    if [[ -z "${GDMDynamic}" ]]; then
        WriteDebug $"GDM Dynamic não encontrado. Por favor, instale-o."
        exit 1
    fi
    
    # Verifica se o Xephyr está instalado
    if [[ -z "${Xephyr}" ]]; then
        WriteDebug $"Xephyr não encontrado. Por favor, instale-o."
        exit 1
    fi
    
    # Verifica se o X está instalado
    if [[ -z "${X}" ]]; then
        WriteDebug $"X não encontrado. Por favor, instale-o."
        exit 1
    fi
    
    # Verifica se o xsetbg está instalado
    if [[ -z "${ShowImage}" ]]; then
        WriteDebug $"xsetbg não encontrado. Instale o pacote xloadimage."
        exit 1
    fi
} # CheckSystem

#
# Esta função verifica se foram definidas algumas das variáveis essenciais
# para a execução das rotinas do MDM. Caso elas não tenham sido definidas nos
# arquivos de configuração, define-as com o valor padrão.
#
    
function InitSystem()
{

    # inicializa o mode de DEBUG como desligado
    if [[ -z "${DEBUG_ENABLED}" ]]; then
        DEBUG_ENABLED=0
    fi
    
    # inicializa o gerenciador de login
    if [[ -z "${DISPLAY_MANAGER}" ]]; then
        DISPLAY_MANAGER="gdm"
    fi
    
    # Cria os diretórios necessários ao MDM
    CreatePath
    
    # Verifica se os programas necessários estão instalados corretamente
    CheckSystem
} # InitSystem

#
# Esta funcão escreve o texto de copyright do sistema mdm
#
# Parâmetros:   nenhum

function MDMCopyright()
{

    echo "Multiseat Display Manager. Version ${VERSION}"
    echo "Copyright (c) 2004-2008 Universidade Federal do Paraná (UFPR)"
    echo "Centro de Computacão Científica e Software Livre (C3SL)"
    echo
    echo "This is free software."
    echo "You may redistribute copies of it under the terms of the"
    echo "GNU General Public License <http://www.gnu.org/licenses/gpl.html>."
    echo "There is NO WARRANTY, to the extent permitted by law."
    echo
    
} # MDMCopyright
