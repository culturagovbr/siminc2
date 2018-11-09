<?php

# Cliente do Webservice
include_once APPRAIZ. 'includes/soap-curl/client.php';

# Contrato de classes
include_once APPRAIZ. 'includes/siop-soap-curl/interface-service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/interface-xml.php';

# Classes abstratas pra implementaчуo de serviчos
include_once APPRAIZ. 'includes/siop-soap-curl/service.php';
include_once APPRAIZ. 'includes/siop-soap-curl/xml.php';

# Classes personalizadas para consumir serviчos
include_once APPRAIZ. 'includes/siop-soap-curl/quantitativo/service/execucao-orcamentaria.php';
include_once APPRAIZ. 'includes/siop-soap-curl/quantitativo/xml/execucao-orcamentaria.php';

