#
#  This file has necessary functions to show images to keyboard and mouse
#  selection for each seat.
#  Este arquivo contém as funcões necessárias para mostrar as imagens para a
#  selecão do teclado e do mouse de cada terminal.
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
#                19/02/2008
#  Dependencies:
#       mdm-path.inc
#       mdm-debug.inc

#
# Esta função mostra uma imagem de acordo com o parâmetro informado
# This function show an image according to given parameter
#
# Parâmetros:   tipo de imagem a ser mostrada
# Parameters:
#                   * keyboard      show image to press Fn
#                   * mouse         show image to press mouse button
#                   * wait          show image to wait
#                   * reconfig      show image to reconfigure seat
#                   * free          exclude temporary images
#
#               número do terminal sendo configurado
#               seat number being configured
#

function SetImage()
{
    local FILES
    local CONVERT_ARGS="-pointsize 22 -fill #000100 -font helvetica-bold"
    local TERMINAL=${2}
    local KEYB_MSG1=$"Press F${TERMINAL} key"
    local KEYB_MSG2=$"to keyboard associated"
    local KEYB_MSG3=$"to this monitor."
    local MOUSE_MSG1=$"Press left mouse button"
    local MOUSE_MSG2=$"associated to this"
    local MOUSE_MSG3=$"monitor"
    local RECONF_MSG1=$"Terminal configurado!"
    local RECONF_MSG1=$"The seat is configured!"
    local RECONF_MSG2=$"Para refazer a configuracão pressione ESC."
    local RECONF_MSG2=$"To reconfigure it, press ESC."
    local RECONF_MSG3=$"Para continuar pressione ENTER."
    local RECONF_MSG3=$"To continue, press ENTER."
    local WAIT_MSG=$"Aguarde..."
    local WAIT_MSG=$"Wait..."
    
    case ${1} in
        free)
            WriteDebug $"Excluindo arquivos temporários de imagem..."
            WriteDebug $"Erasing temporary image files..."
            
            # localiza arquivos temporários de imagem
            # find temporary image files
            for FILES in ${TMP_DIR}/f*.png \
                         ${TMP_DIR}/mouse.png \
                         ${TMP_DIR}/wait.png \
                         ${TMP_DIR}/reconfig.png
            do
                # verifica se é um arquivo comum
                # check if it is a common file
                if [[ -f ${FILES} ]]; then
                    # verifica se o arquivo foi removido e grava no log
                    # check if file was removed and writes on log
                    if rm ${FILES}; then
                        WriteDebug $"Arquivo ${FILES} removido."
                        WriteDebug $"File ${FILES} removed."
                    else
                        WriteDebug $"Erro excluindo arquivo ${FILES}."
                        WriteDebug $"Error while erasing file ${FILES}."
                    fi
                fi
            done
            ;;
        keyboard)
            WriteDebug $"Pressione a tecla F${TERMINAL} ..."
            WriteDebug $"Press F${TERMINAL} key ..."
            
            # verifica se a imagem já existe
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/F${TERMINAL}.png ]]; then
                # imagem não existe. Cria imagem
                # no image. Create it
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 285,365 \"${KEYB_MSG1}\"" \
                           -draw "text 285,390 \"${KEYB_MSG2}\"" \
                           -draw "text 285,415 \"${KEYB_MSG3}\"" \
                           ${IMAGE_DIR}/keyboard.png \
                           ${TMP_DIR}/F${TERMINAL}.png
                 WriteDebug "Imagem para F${TERMINAL}.png criada."
                 WriteDebug $"Image for F${TERMINAL} seat created."
            fi
            # mostra imagem na tela
            # show image on screen
            ${ShowImage} -fullscreen ${TMP_DIR}/F${TERMINAL}.png
            ;;
        mouse)
            WriteDebug $"Imagem: Pressione o botão ... terminal ${TERMINAL}..."
            WriteDebug $"Image: Press button ... seat ${TERMINAL}"
            
            # verifica se a imagem já existe
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/mouse.png ]]; then
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 300,365 \"${MOUSE_MSG1}\"" \
                           -draw "text 300,390 \"${MOUSE_MSG2}\"" \
                           -draw "text 300,415 \"${MOUSE_MSG3}\"" \
                           ${IMAGE_DIR}/mouse.png \
                           ${TMP_DIR}/mouse.png
                WriteDebug "Imagem para mouse.png criada."
                WriteDebug $"mouse.png image created."
            fi
            ${ShowImage} -fullscreen ${TMP_DIR}/mouse.png
            ;;
        wait)
            WriteDebug $"Imagem: Aguarde..."
            WriteDebug $"Image: Wait..."
            
            # verifica se a imagem já existe
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/wait.png ]]; then
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 285,365 \"${WAIT_MSG}\"" \
                           ${IMAGE_DIR}/wait.png \
                           ${TMP_DIR}/wait.png
                WriteDebug "Imagem para wait.png criada."
                WriteDebug $"wait.png image created."
            fi
            ${ShowImage} -fullscreen ${TMP_DIR}/wait.png
            ;;
        reconfig)
            WriteDebug $"Imagem: Pressione ESC..."
            WriteDebug $"Image: Press ESC..."
            
            # verifica se a imagem já existe
            # check if image already exists
            if [[ ! -f ${TMP_DIR}/reconfig.png ]]; then
                ${Convert} ${CONVERT_ARGS} \
                           -draw "text 300,365 \"${RECONF_MSG1}\"" \
                           -draw "text 300,390 \"${RECONF_MSG2}\"" \
                           -draw "text 300,415 \"${RECONF_MSG3}\"" \
                           ${IMAGE_DIR}/reconfig.png \
                           ${TMP_DIR}/reconfig.png 
                WriteDebug $"Imagem para reconfiguracão criada."
                WriteDebug $"Reconfiguration image created."
            fi
            ${ShowImage} -fullscreen ${TMP_DIR}/reconfig.png
            ;;
        *)
            WriteDebug $"SetImage() - Opcão Inválida!!!"
            WriteDebug $"SetImage() - Invalid Option!!!"
            ;;
    esac
} # SetImage
