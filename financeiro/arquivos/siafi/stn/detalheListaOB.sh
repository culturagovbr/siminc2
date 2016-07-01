#!/bin/bash

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
SQLCOPY='sqlCopy/';


FILE=$1;
echo "COPY financeiro.*************(***********) FROM stdin WITH NULL AS '${NULO}';" >> ${FILE}.sql;

#file_length=`wc -l '${FILE}' | cut -c1-5`;


cat ${FILE} | grep ^[^SC*] | while read LINHA;
do 

    ITCOUSUARIO=${CONSTANTE:0:11};
    
    ITCOTERMINALUSUARIO=${LINHA:11:8};

    ITDATRANSACAO=${LINHA:19:8};

    ITHOTRANSACAO=${LINHA:27:4};

    ITCOUGOPERADOR=${LINHA:31:6};

    ITINOPERACAO=${LINHA:37:1};

    GRUGGESTAOANNUMEROLO=${LINHA:38:23};

    ITNUISNDOCHABIL=${LINHA:61:8};

    ITINSITUACAOLISTA=${LINHA:69:1};

    ITVATOTALLISTA=${LINHA:70:17};

    ITCOFAVORECIDO=${LINHA:87:448};

    ITCOGESTAO=${LINHA:537:168};

    GRDOMICILIOFAVORECIDO=${LINHA:703:544};

    ITNULISTA=${LINHA:1247:384};

    ITCOIDENTTRANSFERENCIA=${LINHA:1631:800};

    GRFONTERECURSO=${LINHA:2431:1600};

    ITCOVINCPAGAMENTO=${LINHA:4031:480};

    ITVAFAVORECIDO=${LINHA:4511:2720};

    ITNUOB=${LINHA:7231:384};

    ITNUOBCANCELADA=${LINHA:7615:384};

    GRDOMICILIOPAGADORA=${LINHA:7999:544};

    GRUGGESTAOLISTAORIGEM=${LINHA:8543:23};

    GRUGGESTAOLISTADESTINO=${LINHA:8566:23};

    ITCOUSUARIOTRAN=${LINHA:8589:11};

    ITCOUGOPERADORTRAN=${LINHA:8600:6};

    GRUGGESTAOTRANSFERENCIA=${LINHA:8606:11};

    ITNULISTATRANSFERENCIA=${LINHA:8617:6};

    FILLER=${LINHA:8623:177};


    echo -e $ITCOUSUARIO${TAB}$ITCOTERMINALUSUARIO${TAB}$ITDATRANSACAO${TAB}$ITHOTRANSACAO${TAB}$ITINOPERACAO${TAB}$GRUGGESTAOANNUMEROLO${TAB}$ITNUISNDOCHABIL${TAB}$ITINSITUACAOLISTA${TAB}$ITVATOTALLISTA${TAB}$ITCOFAVORECIDO${TAB}$ITCOGESTAO${TAB}$GRDOMICILIOFAVORECIDO${TAB}$ITNULISTA${TAB}$ITCOIDENTTRANSFERENCIA${TAB}$GRFONTERECURSO${TAB}$ITCOVINCPAGAMENTO${TAB}$ITVAFAVORECIDO${TAB}$ITNUOB${TAB}$ITNUOBCANCELADA${TAB}$GRDOMICILIOPAGADORA${TAB}$GRUGGESTAOLISTAORIGEM${TAB}$GRUGGESTAOLISTADESTINO${TAB}$ITCOUSUARIOTRAN${TAB}$ITCOUGOPERADORTRAN${TAB}$GRUGGESTAOTRANSFERENCIA${TAB}$ITNULISTATRANSFERENCIA${TAB}$FILLER >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

