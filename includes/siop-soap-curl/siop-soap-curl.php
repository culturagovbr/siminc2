<?php

# Cliente do Webservice
include_once 'soap-curl/client.php';

# Contrato de classes
include_once 'interface-service.php';
include_once 'interface-xml.php';

# Cliente do Webservice
include_once 'soap-curl/client.php';

# Classes abstratas pra implementaчуo de serviчos
include_once 'siop-soap-curl/service.php';
include_once 'siop-soap-curl/xml.php';

# Classes personalizadas para consumir serviчos
include_once 'siop-soap-curl/quantitativo/service/execucao-orcamentaria.php';
include_once 'siop-soap-curl/quantitativo/xml/execucao-orcamentaria.php';

