<input type="hidden" name="req" id="req" value=""/>


<div class="ibox-title">
    <h5>Relatório Geral de Emendas</h5><br>
</div>
<div class="ibox-content">
    <?php
        $stSql = "SELECT prsano AS codigo, prsano AS descricao FROM emendas.emenda WHERE emestatus = 'A' union select '".$_SESSION['exercicio']."' as codigo, '".$_SESSION['exercicio']."' as descricao ORDER BY 1;";
        $preenchidos = null;
        if(!$_REQUEST['prsano'] ){
            $preenchidos = array( array('codigo' => $_SESSION['exercicio'], 'descricao' => $_SESSION['exercicio'] ) );
            $_REQUEST['prsano'][] = $_SESSION['exercicio'];
        }else{
            if( count($_REQUEST['prsano']) > 0){
                $preenchidos = array();
                foreach ($_REQUEST['prsano'] AS $ano){
                    $preenchidos[] = array('codigo' => $ano, 'descricao' => $ano );
                }
            }
        }
        echo $simec->select('filtro[eme.prsano][]', 'Exercício', $preenchidos, $stSql, null, ['input-size'=>9, 'label-size'=>2]);
    ?>

    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Agrupadores:</label>
    <?php
        // Início dos agrupadores
        $agrupador = new AgrupadorBootstrap('filtro', '');
        // Dados padrão de destino (nulo)
        $destino = isset($agrupador2) ? $agrupador2 : array();
        // Dados padrão de origem
        $origem = array(array('codigo' => 'emeid',                     'descricao' => 'ID Emenda'),
                        array('codigo' => 'emenumero',                 'descricao' => 'Número'),
                        array('codigo' => 'autcod',                    'descricao' => 'Cod. Autor'),
                        array('codigo' => 'autnome',                   'descricao' => 'Autor'),
                        array('codigo' => 'autsexo',                   'descricao' => 'Gênero'),
                        array('codigo' => 'atpdsc',                    'descricao' => 'Tipo Parlamentar'),
                        array('codigo' => 'estuf',                     'descricao' => 'UF Parlamentar'),
                        array('codigo' => 'parsigla',                  'descricao' => 'Partido'),
                        array('codigo' => 'unocod',                    'descricao' => 'Cod. Unidade'),
                        array('codigo' => 'unosigla',                  'descricao' => 'Unidade'),
                        array('codigo' => 'funcional',                 'descricao' => 'Funcional'),
                        array('codigo' => 'benid',                     'descricao' => 'Id Beneficiario'),
                        array('codigo' => 'suocod',                    'descricao' => 'Código Subunidade'),
                        array('codigo' => 'suonome',                   'descricao' => 'Subunidade'),
                        array('codigo' => 'suocoddelegada',            'descricao' => 'Código Subunidade delegada'),
                        array('codigo' => 'suonomedelegada',           'descricao' => 'Subunidade delegada'),
                        array('codigo' => 'mdedsc',                    'descricao' => 'Área Cultural'),
                        array('codigo' => 'needsc',                    'descricao' => 'Segmento Cultural'),
                        array('codigo' => 'alodsc',                    'descricao' => 'Alterações'),
                        array('codigo' => 'benparecertecnico',         'descricao' => 'Parecer Técnico'),
                        array('codigo' => 'benparecerjuridico',        'descricao' => 'Parecer Jurídico'),
                        array('codigo' => 'benimpedimento',            'descricao' => 'Impedimento'),
                        array('codigo' => 'impdsc',                    'descricao' => 'Motivo'),
                        array('codigo' => 'situacao_proposta',         'descricao' => 'Situação Proposta'),
                        array('codigo' => 'esddsc',                    'descricao' => 'Situação'));
        // exibe agrupador
        $agrupador->setOrigem('naoAgrupador', null, $origem);
        $agrupador->setDestino('agrupador', null, $destino);
        $agrupador->exibir();
    ?>


    <label class="col-sm-2 col-md-2 col-lg-2 control-label">Colunas</label>
    <?php
        unset($agrupador, $destino, $origem);
        // Início das colunas
        $agrupador = new AgrupadorBootstrap('filtro', '');
        $agrupador->width = 345;
        // Dados padrão de destino (nulo)
        $destino = isset($agrupador2) ? $agrupador2 : array();
        // Dados padrão de origem
        $origem = array(array('codigo' => 'emeimpositiva',             'descricao' => 'Impositiva'),
                        array('codigo' => 'emejustificativa',          'descricao' => 'Justificativa'),
                        array('codigo' => 'emdvalorcusteio',           'descricao' => 'Valor Custeio'),
                        array('codigo' => 'emdvalorcapital',           'descricao' => 'Valor Capital'),
                        array('codigo' => 'procnpj',                   'descricao' => 'CNPJ'),
                        array('codigo' => 'pronome',                   'descricao' => 'Proponente'),
                        array('codigo' => 'esfdsc',                    'descricao' => 'Localização'),
                        array('codigo' => 'paidescricao',              'descricao' => 'Paises'),
                        array('codigo' => 'estuf',                     'descricao' => 'UFs'),
                        array('codigo' => 'mundescricao',              'descricao' => 'Municipios'),
                        array('codigo' => 'bennumeroprocesso',         'descricao' => 'Número do Processo'),
                        array('codigo' => 'beninicio',                 'descricao' => 'Data de Início da Vigência'),
                        array('codigo' => 'capdsc',                    'descricao' => 'Modalidade de Pactuação'),
                        array('codigo' => 'bented',                    'descricao' => 'TED'),
                        array('codigo' => 'benmotivoimpedimento',      'descricao' => 'Justivicativa'),
                        array('codigo' => 'bedvalorcusteio',           'descricao' => 'Vl. Beneficiário Custeio'),
                        array('codigo' => 'bedvalorcapital',           'descricao' => 'Vl. Beneficiário Capital'),
                        array('codigo' => 'bedvalorpriorizadocusteio', 'descricao' => 'Vl. Priorizado Custeio'),
                        array('codigo' => 'bedvalorpriorizadocapital', 'descricao' => 'Vl. Priorizado Capital'),
                        array('codigo' => 'contigenciadocusteio',      'descricao' => 'Vl. Contingenciado Custeio'),
                        array('codigo' => 'contigenciadocapital',      'descricao' => 'Vl. Contingenciado Capital'),
                        array('codigo' => 'numero_programa',           'descricao' => 'Número Programa - SICONV'),
                        array('codigo' => 'titulo_programa',           'descricao' => 'Título Programa - SICONV'),
                        array('codigo' => 'objeto_programa',           'descricao' => 'Objeto Programa'),
                        array('codigo' => 'numero_proposta',           'descricao' => 'Número Proposta - SICONV'),
                        array('codigo' => 'titulo_proposta',           'descricao' => 'Título Proposta - SICONV'),
                        array('codigo' => 'objeto_proposta',           'descricao' => 'Objeto Proposta'),
                        array('codigo' => 'pliid',                     'descricao' => 'Id PI'),
                        array('codigo' => 'plicod',                    'descricao' => 'Código PI'),
                        array('codigo' => 'empenhado',                 'descricao' => 'Empenhado'),
                        array('codigo' => 'pago',                      'descricao' => 'Pago'));
        // exibe agrupador
        $agrupador->setOrigem('naoColuna', null, $origem);
        $agrupador->setDestino('coluna', null, $destino);
        $agrupador->exibir();
    ?>
    <?php
        $strAno = "";
        $strDsc = "";

        if (count($_REQUEST['prsano']) == 1) {
            $strAno = " uno.prsano IN ('" . implode("','", $_REQUEST['prsano']) . "')  AND ";
            $strDsc = " uno.unocod || ' - ' || uno.unonome  ";
        } else {
            $strDsc = " '(Exercicio ' || uno.prsano || ') ' || uno.unocod || ' - ' || uno.unonome ";
            if (count($_REQUEST['prsano']) > 0) {
                $strAno = " uno.prsano IN ('" . implode("','", $_REQUEST['prsano']) . "')  AND ";
            } else {
                $strAno = " ";
            }
        }

        $stSql = "  SELECT DISTINCT uno.unoid AS codigo,
                           {$strDsc} AS descricao,
                           uno.prsano
                      FROM public.unidadeorcamentaria uno
                     INNER JOIN public.subunidadeorcamentaria suo ON suo.unoid = uno.unoid
                     WHERE {$strAno} uno.unostatus = 'A'
                    {$whereResponsabilidade}
                     ORDER BY 3,2";
        echo $simec->select("filtro[eme.unoid][]", 'Unidade', null, $stSql, null, ['input-size'=>9, 'label-size'=>2]);
    ?>
    <div class="text-center">
        <a class="btn-success btn btn-md" href="javascript:exibeRelatorioGeral('exibir');">Visualizar</a>
        <a class="btn-info btn btn-md" href="javascript:exibeRelatorioGeral('xls');">Visualizar XLS</a>
    </div>
</div>
