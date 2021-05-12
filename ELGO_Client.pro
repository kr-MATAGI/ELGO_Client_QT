TEMPLATE = subdirs

SUBDIRS = EFC \
        elgo_control \
        elgo_main \
        elgo_update \
        elgo_viewer

EFC.file = EFC/EFC.pro
elgo_contorl.file = elgo_control/elgo_control.pro
elgo_main.file = elgo_main/elgo_main.pro
elgo_viewer.file = elgo_viewer/elgo_viewer.pro
elgo_update.file = elgo_update/elgo_update.pro
