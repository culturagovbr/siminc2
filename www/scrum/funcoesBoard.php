<?php
/**
 * Sistema SCRUM
 * @package simec
 * @subpackage scrum
 */

/**
 * Cor do postit da coluna sprint.
 */
define('POSTIT_SPRINT', 'green');
/**
 * Cor do postit da coluna de pr�xima sprint.
 */
define('POSTIT_PROXIMA_SPRINT', 'yellow');
/**
 * Coro do postit da coluna de backlog.
 */
define('POSTIT_BACKLOG', 'blue');

/**
 * Cria o t�tulo da coluna sprint.
 * A quantidade de horas � valida tanto para a coluna sprint, qto para a
 * coluna pr�xima strint.
 * 
 * @param int $totalHoras Quantidade total de horas da sprint.
 * @param int $horasGastas Quantidade de horas alocanadas na sprint.
 * @return string
 */
function tituloSprint($totalHoras, $horasGastas)
{
    return <<<TITULO
Sprint (<span id="sprintHora">{$horasGastas}</span>hs / <span id="totalHoras">{$totalHoras}</span>hs)
TITULO;
}

/**
 * Cria o t�tulo da coluna pr�xima sprint.
 * 
 * @param int $totalHoras Quantidade total de horas da pr�xima sprint.
 * @param int $horasGastas Quantidade de horas alocanadas na sprint.
 * @return string
 */
function tituloProximaSprint($totalHoras, $horasGastas)
{
    return <<<TITULO
Pr�xima Sprint (<span id="proximasprintHora">{$horasGastas}</span>hs / {$totalHoras}hs)
TITULO;
}

/**
 * Corta uma palavras e concatena um delimitador
 * 
 * @param string $value
 * @param int $limit
 * @param string $delimiters
 * @return string
 */
function cortaPalavra($value, $limit = '21', $delimiters = '...') {

    if (strlen($value) > $limit) {
        $value = substr($value, 0, 21).$delimiters;
    }
    
    return $value;
}

/**
 * Cria um postit, um item das colunas.
 * 
 * @param array $item Informa��es gerais do item.
 * @param string $class Nome da classe que ir� dar a cor para o postit.
 * @return string Conjunto de divs e informa��es que comp�em o postit.
 */
function criaPostit($item, $class)
{
    $item['usucpfresp_dsc'] = cortaPalavra($item['usucpfresp_dsc']);
    
    return <<<POSTIT
<div class="postit ui-corner-all {$class}" id="eid{$item['entid']}">
    <div class="header">
        <div class="loading"><img src="css/imagens/loading.gif" /></div>
        <div class="subprograma" style="color:{$item['subprgcolor']}">{$item['subprgdsc']}</div>
        <div class="duracao">
            <span class="hora">{$item['enthrsexec']}</span>hs
        </div>
    </div>
    <div class="body">
        <div class="content" style="border-left-color:{$item['subprgcolor']}">
            <span class="estoria">{$item['esttitulo']}</span>
            {$item['entdsc']}
        </div>
        <div class="toolbar" id="tb{$item['entid']}">
            <img src="css/imagens/ver.png" title="visualizar entreg�vel" style="display:none" />
            <img src="css/imagens/editar.png" class="edit-item" title="editar entreg�vel" />
            <img src="css/imagens/comentario.png" title="adicionar coment�rio" style="display:none" />
            <img src="css/imagens/upload.png" title="adicionar anexo" style="display:none" />
            <img src="css/imagens/responsavel.png" title="definir respons�vel" style="display:none" />
            <img src="css/imagens/status-cancelado.png" title="entreg�vel cancelado" style="float:right;display:none" />
            <img src="css/imagens/status-finalizado.png" title="entreg�vel finalizado" style="float:right;display:none" />
            <img src="css/imagens/status-iniciado.png" title="entreg�vel em execu��o" style="float:right;display:none" />
            <img src="css/imagens/status-nao-iniciado.png" title="entreg�vel n�o iniciado" style="float:right;display:none" />
        </div>
    </div>
    <div class="footer">
        <span class="id">#{$item['entid']}</span>
        <div class="responsavel ui-corner-all">{$item['usucpfresp_dsc']}</div>
    </div>
</div>
POSTIT;
}
