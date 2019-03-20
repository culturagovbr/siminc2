<?php
/**
 * Created by PhpStorm.
 * User: RuySilva
 * Date: 19/05/14
 * Time: 18:52
 */
//global $db;

function removeSlashes(&$data)
{
    if (is_array($data)) {
        foreach ($data as &$value) {
            removeSlashes($value);
        }
    } else if (!empty($data)) {
        $data = stripslashes($data);
    }
}


if ($_POST['listingAjax']) {

    if ($_POST['dataAjax']) {

        $listing = new Listing();

        if (!empty($_POST['dataAjax']) && is_string($_POST['dataAjax'])) {
            $_POST['dataAjax'] = base64_decode($_POST['dataAjax']);
        }

        if (!empty($_POST['perPageAjax']) && is_string($_POST['perPageAjax'])) {
            $listing->setPerPage($_POST['perPageAjax']);
        }

        if (!empty($_POST['pageNumber']) && is_string($_POST['pageNumber'])) {
            $listing->setPageNumber($_POST['pageNumber']);
        }

        if (!empty($_POST['fieldAjax']) && is_string($_POST['fieldAjax'])) {
            $listing->setField($_POST['fieldAjax']);
//            ver($_POST['fieldAjax'] );
        }

        if (!empty($_POST['enableCount']) && is_string($_POST['enableCount'])) {
            if ($_POST['enableCount'] == 'true') {
                $listing->enableCount(true);
            } else {
                $listing->enableCount(false);
            }
        }

        if (!empty($_POST['headAjax']) && is_array($_POST['headAjax'])) {

//            foreach($_POST['headAjax'] as &$val){
//                $val = utf8_encode($val);
//            }

            $listing->setHead($_POST['headAjax']);
        }

        if (!empty($_POST['actionsAjax']) && is_array($_POST['actionsAjax'])) {
            $listing->setActions($_POST['actionsAjax']);
        }

        if ($_POST['pageAjax']) {
            $listing->setPage($_POST['pageAjax']);
        }

        removeSlashes($_POST['dataAjax']);

        if ($_POST['typePageAjax'] && $_POST['typePageAjax'] != 'P') {
            $listing->setTypePage($_POST['typePageAjax']);
            $listing->loadData($_POST['dataAjax']);
            $listing->page();
        } else {
            $listing->listing($_POST['dataAjax']);
        }

        exit;
    } else {

        ver('Dados incorretos para a lisagem com ajax', d);

    }
}


class Listing
{

    const K_TYPE_PAGE_PAGINATOR = 'P';
    const K_TYPE_PAGE_MORE = 'M';
    const K_TYPE_PAGE_SCROLL = 'S';

    private $_data;
    private $_dataFull;
    private $_head;
    private $_perPage = 10;
    private $_page = 1;
    private $_typePage;
    private $_sql;
    private $_offSet;
    private $_onSet;
    private $_pagination;
    private $_total;
    private $_pageNumber = 7;
    private $_field = '';
    private $_fieldOrder = 1;
    private $_idList;
    private $_enableCount = false;
    private $_enablePagination= true;
    private $_actions = array();
    private $onclick = true;
    private $idTable = '';
    private $classTable = 'table table-striped table-condensed table-bordered';
    private $_actionMsg = array(
		'edit'     => 'Editar documento',
		'delete'   => 'Excluir documento',
		'file'     => 'Salvar arquivo',
		'envelope' => 'Enviar e-mail'
    );

    public function __construct($onclick = true)
    {
//        $this->setData($data);

//        $this->setLimit( $this->getPage() * $this->getPerPage() );
//        $this->setOffset( $this->getLimit() - $this->getPerPage() + 1 );
        $this->onclick = $onclick;
        $this->setPage($this->getPage());
    }


    /**
     * Se for sql, n?o passar LIMIT ou OFFSET
     *
     * @return mixed
     */
    public function enableCount($value)
    {
        $this->_enableCount = $value;
        return $this;
    }

    /**
     * Se for sql, n?o passar LIMIT ou OFFSET
     *
     * @return mixed
     */
    public function setEnablePagination($value)
    {
        $this->_enablePagination = $value;
        return $this;
    }

    public function getEnablePagination()
    {
        return $this->_enablePagination;
    }

    public function setData(array $value)
    {
        $this->_data = $value;
        return $this;
    }

    public function getData()
    {
        return $this->_data;
    }

    public function setIdTable($value)
    {
        $this->idTable = $value;
        return $this;
    }

    public function getIdTable()
    {
        return $this->idTable;
    }

    public function setClassTable($value)
    {
        $this->classTable = $value;
        return $this;
    }

    public function getClassTable()
    {
        return $this->classTable;
    }


    public function array_orderby()
    {
        $args = func_get_args();
        $data = array_shift($args);
        foreach ($args as $n => $field) {
            if (is_string($field)) {
                $tmp = array();
                foreach ($data as $key => $row)
                    $tmp[$key] = $row[$field];
                $args[$n] = $tmp;
            }
        }
        $args[] = & $data;
        call_user_func_array('array_multisort', $args);
        return array_pop($args);
    }

    public function loadData($value)
    {
        global $db;

        $field = $this->getField();

        if (is_array($value)) {

            $fields = array();
            $n = 1;
            foreach ($value[0] as $key => $fieldName) {
                $fields[$n] = $key;
                $n++;
            }
            if ($this->getField()) {
                $value = $this->array_orderby($value, $fields[$this->getField()], SORT_ASC);
//                ver($fields[$this->getField()] , $value );
            }

            $data = array();
            $n = 1;
            $loop = 1;
            foreach ($value as $row) {
                $data[$n][$loop] = $row;

                if ($loop >= $this->getPerPage()) {
                    $loop = 1;
                    $n++;
                } else {
                    $loop++;
                }
            }

            $this->setData($data[$this->getPage()]);
            $this->setDataFull($value);
            $this->setTotal(count($value));
            $this->loadPagination();


        } else if (is_string($value) && !empty($value)) {

            if ($field) {
                if (stristr($value, 'ORDER BY')) {
                    $arrValue = explode('ORDER BY', $value);
                    $value = $arrValue[0];
                }
                $value = $value . "ORDER BY {$field}";
            }


            $sql = $value . ' OFFSET ' . $this->getOffSet() . ' LIMIT ' . $this->getPerPage();
//            ver($sql, d);
            $data = $db->carregar($sql);

            if ($data) {
                $sqlTotal = "SELECT count(*) as tot FROM ({$value}) as foo";
                //            $sqlTotal = preg_replace( array('/SELECT/' , '/FROM/'), array(' SELECT count( ' , ' ) FROM '), $value , 1 );
                $total = $db->pegaUm($sqlTotal);
                $this->setData($data);
            }

            $this->setSql($value);
            $this->setDataFull($data);
            $this->setTotal($total);
            $this->loadPagination();
        }
    }

    public function loadPagination()
    {
        $total = $this->getTotal();

        if ($total) {

            $pageNumber = $this->getPageNumber();
            $perPage = $this->getPerPage();
            $page = $this->getPage();

            $pages = (int)($total / $perPage);
            $pages += ($total % $perPage > 0 ? 1 : 0);

            $prevAll = 1;
            $prev = $page - 1;
            $next = $page + 1;
            $nextAll = $pages;

            $currentPaginations = (int)($page - ($pageNumber / 2) + 1);
            for ($i = 1; $i <= $pageNumber; $i++) {
                if ($currentPaginations < 1) {
                    $currentPaginations++;
                }
            }

            $pagination = array();
            for ($i = 1; $i <= $pageNumber; $i++) {
                $pagination[$i] = $currentPaginations;
                $currentPaginations++;
                if ($currentPaginations > $nextAll) {
                    break;
                }
            }

            $pagination = array(
                'prevAll' => $prevAll
            , 'prev' => $prev
            , 'pagination' => $pagination
            , 'next' => $next
            , 'nextAll' => $nextAll
            );

            $this->setPagination($pagination);
        } //else ver('Não possui total para gerar a paginação!', d);
    }

    public function setPageNumber($value)
    {
        $this->_pageNumber = $value;
        return $this;
    }

    public function getPageNumber()
    {
        return $this->_pageNumber;
    }

    public function setDataFull($value)
    {
        $this->_dataFull = $value;
        return $this;
    }

    public function getDataFull()
    {
        return $this->_dataFull;
    }

    public function setHead(array $value)
    {
        $this->_head = $value;
        return $this;
    }

    public function getHead()
    {
        return $this->_head;
    }

    public function setPerPage($value)
    {
        $this->_perPage = $value;
        return $this;
    }

    public function getPerPage()
    {
        return $this->_perPage;
    }

    public function setField($value)
    {
        $this->_field = $value;
        return $this;
    }

    public function getField()
    {
        return $this->_field;
    }

    public function setTypePage($value)
    {
        $this->_typePage = $value;
        return $this;
    }

    public function getTypePage()
    {
        return $this->_typePage;
    }

    public function setFieldOrder($value)
    {
        $this->_fieldOrder = $value;
        return $this;
    }

    public function getFieldOrder()
    {
        return $this->_fieldOrder;
    }

    public function setPage($value)
    {
        $this->setOnSet($value * $this->getPerPage() - 1);
        $this->setOffSet($this->getOnSet() - $this->getPerPage() + 1);
        $this->_page = $value;
        return $this;
    }

    public function getPage()
    {
        return $this->_page;
    }

    public function setOnSet($value)
    {
        $this->_onSet = $value;
        return $this;
    }

    public function getOnSet()
    {
        return $this->_onSet;
    }

    public function setPagination(array $value)
    {
        $this->_pagination = $value;
        return $this;
    }

    public function getPagination()
    {
        return $this->_pagination;
    }

    public function setTotal($value)
    {
        $this->_total = $value;
        return $this;
    }

    public function getTotal()
    {
        return $this->_total;
    }

    public function setOffSet($value)
    {
        $this->_offSet = $value;
        return $this;
    }

    public function getOffSet()
    {
        return $this->_offSet;
    }

    public function setActions(array $value)
    {
        $this->_actions = $value;
        return $this;
    }

    public function getActions()
    {
        return $this->_actions;
    }

    public function setSql($value)
    {
        if (!empty($value) && is_string($value)) {
            $this->_sql = $value;
            return $this;
        } else ver('O sql está vazio ou não é uma string!', $value, d);
    }

    public function getSql()
    {
        return $this->_sql;
    }

    private function generateIdList()
    {
        $this->setIdList('listing_' . rand() . '_' . rand());
    }

    public function setIdList($value)
    {
        $this->_idList = $value;
        return $this;
    }

    public function getIdList()
    {
        return $this->_idList;
    }

    public function listing($data)
    {
		$this->loadData($data);
		$data = $this->getData();

        if ($data && is_array($data) && count($data) > 0):
			$head = $this->getHead();
			$this->generateIdList();
			$idList = $this->getIdList();
			$actions = $this->getActions();
            ?>
            <style>
                table thead tr:first-child th:first-child {
                    .border-radius(4px 0 0 0);
                }

                table thead tr:first-child th:first-child {
                    .border-radius(0 4px 0 0);
                }
            </style>
            <div class="row container-listing" id="<?php echo $idList; ?>">
                <div class="col-lg-12">
                    <!--                    table-hover-->
                    <table class="<?= $this->getClassTable() ?>" id="<?= $this->getIdTable() ?>">
                        <?php if ($head): ?>
                            <thead class="btn-primary" style=" -moz-border-radius:10px;
                          -webkit-border-radius:10px;
                          border-radius:10px;
                          border-collapse: separate !important;
                          height: 30px;

                            "
                                >
<!--                            <tr style="height: 33px;">-->
                                <?php if ($this->_enableCount): ?>
                                    <th>#</th>
                                <?php endif; ?>
                                <?php if ($actions): ?>
                                    <th class="text-center" colspan="<?php echo count($actions) ?>">Ação</th>
                                <?php endif; ?>
                                <?php foreach ($head as $key => $value): ?>
                                    <th style="cursor: pointer;" class="load-listing-ajax-order" field="<?php echo $key + 1; ?>">
                                        <?php echo $value ?>
                                        <!--                                <span class="caret"></span>-->
                                    </th>
                                <?php endforeach; ?>
<!--                            </tr>-->
                            </thead>
                        <?php endif; ?>
                        <tbody>
                        <?php $n = $this->getOffset() + 1; ?>
                        <?php foreach ($data as $indice => $dataValue): ?>
                            <!-- <tr class="info">-->
                            <tr id="<?= 'tr_'.$this->getIdTable().'_'. reset( $dataValue ) ; ?>">
                                <?php if ($this->_enableCount): ?>
                                    <td class="text-center"><?php echo $n ?></td>
                                <?php endif; ?>
                                <?php $this->renderActionList($dataValue) ?>
                                <?php foreach ($dataValue as $key => $value) : ?>
                                    <td><?php echo $value ?></td>
                                <?php endforeach; ?>
                            </tr>
                            <?php $n++; ?>
                        <?php endforeach; ?>
                        </tbody>
                    </table>
                </div>
                <?php if($this->getEnablePagination()) $this->loadTypePage(); ?>
            </div>
        <?php else: ?>
            <div class="alert alert-warning text-center">
                <!--                <strong>Desculpe!</strong> -->
                Nenhum resultado foi encontrado!
            </div>
        <?php endif;
    }

    public function listing_tabulado($data, $color_pair = '', $color_odd = '', $color_table_border = '', $color_th = '' )
    {
        $this->loadData($data);
        $data = $this->getData();

        if ($data && is_array($data) && count($data) > 0):
			$head = $this->getHead();
			$this->generateIdList();
			$idList = $this->getIdList();
			$actions = $this->getActions();
            ?>
            <style>
                table thead tr:first-child th:first-child { .border-radius(4px 0 0 0); }
                table thead tr:first-child th:first-child { .border-radius(0 4px 0 0); }
                <?php 
                    echo $color_th;
                    echo $color_table_border;
                ?>

            </style>
            <div class="row container-listing" id="<?php echo $idList; ?>">
                <div class="col-lg-12">
                    <!--                    table-hover-->
                    <table class="<?= $this->getClassTable() ?>" id="<?= $this->getIdTable() ?>"  width="95%" cellspacing="0" cellpadding="2" align="center">
                        <?php if ($head): ?>
                            <thead class="btn-primary" style=" -moz-border-radius:10px; -webkit-border-radius:10px; border-radius:10px; border-collapse: separate !important; height: 30px;" >
<!--                            <tr style="height: 33px;">-->
                                <?php if ($this->_enableCount): ?>
                                    <th>#</th>
                                <?php endif; ?>
                                <?php if ($actions): ?>
                                    <th class="text-center" colspan="<?php echo count($actions) ?>">Ação</th>
                                <?php endif; ?>
                                <?php foreach ($head as $key => $value): ?>
                                    <th style="" class="mescla load-listing-ajax-order" field="<?php echo $key + 1; ?>">
                                        <?php echo $value ?>
                                        <!--                                <span class="caret"></span>-->
                                    </th>
                                <?php endforeach; ?>
<!--                            </tr>-->
                            </thead>
                        <?php endif; ?>
                        <tbody>
                        <?php 
                                $n = $this->getOffset() + 1; 
                                $c = 0;
                        ?>
                        <?php foreach ($data as $indice => $dataValue): 
                            if (fmod($c,2) == 0) $marcado = "$color_pair" ; else $marcado="$color_odd" ?>
                            <!-- <tr class="info">-->
                            <tr id="<?= 'tr_'.$this->getIdTable().'_'. reset( $dataValue ) ; ?>" <?php echo $marcado;?>>
                                <?php if ($this->_enableCount): ?>
                                    <td class="text-center"><?php echo $n ?></td>
                                <?php endif; ?>
                                <?php $this->renderActionList($dataValue) ?>
                                <?php foreach ($dataValue as $key => $value) : ?>
                                    <td><?php echo $value ?></td>
                                <?php endforeach; ?>
                            </tr>
                            <?php $n++; $c++;?>
                        <?php endforeach; ?>
                        </tbody>
                    </table>
                </div>
                <?php if($this->getEnablePagination()) $this->loadTypePage(); ?>
            </div>
        <?php else: ?>
            <div class="alert alert-warning text-center">
                <!--                <strong>Desculpe!</strong> -->
                Nenhum resultado foi encontrado!
            </div>
        <?php endif;
    }

    private function renderActionList(&$dataValue)
    {
        $actions = $this->getActions();
		$_actionMsg = $this->_actionMsg;
        if ($actions):
            $id = array_shift($dataValue);
            ?>
            <?php foreach ($actions as $actionKey => $action): ?>
            <?php if ($action): ?>
                <td style="width: 5%;" class="text-center">
                    <?php
                    switch ($actionKey) {
                        case 'edit':
                            $icon = 'pencil';
                            break;
                        case 'delete':
                            $icon = 'remove';
                            break;
                        default:
                            if (empty($actionKey)) {
                                $icon = '';
                            } else {
                                $icon = $actionKey;
                            }
                            break;
                    }
                    $onclick = '';
                    if ($this->onclick) {
                        $onclick = " onclick='javascript:{$action}( \"" . trim($id) . "\", this)'";
                    }
					$title = " title=\"{$_actionMsg[$actionKey]}\"";

					?>

                    <a class="btn_<?= $action ?>" href="javascript:void(0);" <?= $title ?> <?= $onclick ?> data-id="<?= $id ?>">
                        <i class="glyphicon glyphicon-<?php echo $icon ?>"></i>
                    </a>
                </td>
            <?php endif ?>
        <?php endforeach ?>
        <?php endif ?>
    <?php
    }

    private function loadTypePage()
    {
        switch ($this->getTypePage()) {
            case self::K_TYPE_PAGE_PAGINATOR:
                $this->typePagePaginator();
                break;
            case self::K_TYPE_PAGE_MORE:
                $this->typePageMore();
                break;
            case self::K_TYPE_PAGE_SCROLL:
                $this->typePageScroll();
                break;
            default:
                $this->typePagePaginator();
                break;
        }
    }

    private function typePagePaginator()
    {
        $pageCurrent = $this->getPage();
        $pagination = $this->getPagination();
        $sql = $this->getSql();
        $idList = $this->getIdList();
        $actions = $this->getActions();

        $head = $this->getHead();
//        foreach($head as &$val){
//            $val = utf8_encode($val);
//            echo $val ;
//        }

        if ( !empty($sql) && is_string($sql)) {
            $data = "$.parseJSON('" . simec_json_encode(base64_encode($sql)) . "')";
        } else {
			$d = $this->getDataFull();
			$d = utf8_encode_recursive($d);
            $data = simec_json_encode($d);
        }
//ver($this->getHead());
        ?>
        <!--        --><?php //echo simec_json_encode($this->getHead()); ?>
        <div class="col-lg-12 text-center" style="padding-bottom: 20px;">
            <ul class="pagination">
                <li class="<?php echo ($pagination['prev'] < 1) ? 'disabled' : 'load-listing-ajax' ?>" page="<?php echo $pagination['prev'] ?>">
                    <a href="javascript:void(0);">&laquo;</a></li>
                <?php if ($pagination['prev'] > 2): ?>
                    <li class="load-listing-ajax" page="<?php echo $pagination['prevAll'] ?>">
                        <a href="javascript:void(0);">&laquo; <?php echo $pagination['prevAll'] ?></a></li>
                <?php endif ?>
                <?php foreach ($pagination['pagination'] as $page): ?>
                    <li class="load-listing-ajax <?php if ($page == $pageCurrent) echo 'active' ?>" page="<?php echo $page ?>">
                        <a href="javascript:void(0);"><?php echo $page ?> </a></li>
                <?php endforeach; ?>
                <li class="<?php echo ($pagination['next'] > $pagination['nextAll']) ? 'disabled' : 'load-listing-ajax' ?>" page="<?php echo $pagination['next'] ?>">
                    <a href="javascript:void(0);">&raquo;</a></li>
            </ul>
        </div>
        <!--        --><?php //echo simec_json_encode($data); ?>
        <script language="JavaScript">
            //            container-listing
            var field = '<?php echo $this->getField(); ?>';

            $('#<?php echo $idList; ?> .load-listing-ajax').click(function () {

                var button = $(this)
                var page = button.attr('page');

                data = {
                    listingAjax: true, pageAjax: page, fieldAjax: field, actionsAjax: $.parseJSON('<?php echo simec_json_encode($actions); ?>'), perPageAjax: <?php echo $this->getPerPage() ?>, pageNumber: <?php echo $this->getPageNumber() ?>, dataAjax: <?php echo $data; ?>, headAjax: $.parseJSON('<?php echo simec_json_encode($head); ?>')
                };

//                $.post(window.location.href , data , function(html){
//                    button.closest('.container-listing').replaceWith(function(){
//                        return $(html).hide().fadeIn();
//                    });
//                });

                $.ajax({
                    type: 'POST',
                    url: window.location.href,
                    data: data,
                    success: function (html) {
                        button.closest('.container-listing').replaceWith(function () {
                            return $(html).hide().fadeIn();
                        });
                    }
//                    , dataType: 'application/json; charset=utf-8'
//                    , async:false
                });


//                $.ajaxSetup({async: false});
                console.info(data);
            });

            $('#<?php echo $idList; ?> .load-listing-ajax-order').click(function () {
                var element = $(this);
                field = element.attr('field');
                var page = <?php echo $this->getPage(); ?>;

                data = {
                    listingAjax: true, actionsAjax: $.parseJSON('<?php echo simec_json_encode($actions); ?>'), typePageAjax: '<?php echo $this->getTypePage() ?>', perPageAjax: '<?php echo $this->getPerPage() ?>', pageNumber: '<?php echo $this->getPageNumber() ?>', pageAjax: 1, fieldAjax: field, dataAjax: <?php echo $data; ?>, headAjax: $.parseJSON('<?php echo simec_json_encode($this->getHead()); ?>')
                    <!--                    , enableCounts : '--><?php //echo ($this->_enableCount())? 'true' : 'false'; ?><!--'-->
                };
//                console.info(data);

//                $.post(window.location.href , data , function(html){
//                    element.closest('.container-listing').hide().html(html).fadeIn();
////                    button.closest('.container-listing').after(html);
//                });
            });
        </script>
    <?php
    }

    private function typePageMore()
    {
        $sql = $this->getSql();

        if (!empty($sql) && is_string($sql)) {
            $data = "$.parseJSON('" . simec_json_encode(base64_encode($sql)) . "')";
        } else {
			$d = $this->getDataFull();
			$d = utf8_encode_recursive($d);
            $data = simec_json_encode($this->getDataFull());
        }

        $idList = $this->getIdList();
        $actions = $this->getActions();

        $pagination = $this->getPagination();
        $pageCurrent = $this->getPage();

        $btMore = true;
        if ($pageCurrent >= $pagination['nextAll']) {
            $btMore = false;
        }

        if ($btMore):
            ?>
            <div class="col-lg-12 text-center" style="padding-bottom: 20px;">
                <!--        <button type="button" class="btn btn-primary btn-lg btn-block load-listing-ajax">Carregar mais</button>-->
                <div class="btn-group">
                    <!--                <button type="button" onclick="javascript:function(){$('html,body').animate({scrollTop: 0},'slow');};" class="btn btn-default"><i class="glyphicon glyphicon-arrow-up"></i> Subir</button>-->
                    <button <?php if (!$btMore) echo 'disabled="disabled"' ?> type="button" class="btn btn-primary load-listing-ajax">
                        Carregar mais
                    </button>
                </div>
            </div>
            <script language="JavaScript">
                //            container-listing

                var field = '<?php echo $this->getField(); ?>';

                var onSet = <?php echo $this->getPage() ?>;
                var limit = <?php echo $pagination['nextAll'] ?>

                    $('#<?php echo $idList; ?>  .load-listing-ajax').click(function () {

                        onSet++;

                        if (onSet >= limit) {
                            $(this).fadeOut();
                            return false;
                        }

                        var button = $(this);

                        data = {
                            listingAjax: true, typePageAjax: 'M', actionsAjax: $.parseJSON('<?php echo simec_json_encode($actions); ?>'), perPageAjax: <?php echo $this->getPerPage(); ?>, pageNumber: <?php echo $this->getPageNumber() ?>, pageAjax: onSet, fieldAjax: field, dataAjax: <?php echo $data; ?>, headAjax: $.parseJSON('<?php echo simec_json_encode($this->getHead()); ?>')
                            <!--                            , enableCount : --><?php //echo "{$this->_enableCount()}"; ?><!--')-->
                        };
                        console.info(data);
//                $.post(window.location.href , data , function(html){
////                    button.closest('.container-listing').hide().html(html).fadeIn();
//                    button.closest('.container-listing').find('tr:last').after(html);
//                });

                    });

                $('#<?php echo $idList; ?> .load-listing-ajax-order').click(function () {

                    onSet = 1;

                    var element = $(this);
                    field = element.attr('field');
                    var page = <?php echo $this->getPage(); ?>;

                    data = {
                        listingAjax: true, actionsAjax: $.parseJSON('<?php echo simec_json_encode($actions); ?>'), typePageAjax: '<?php echo $this->getTypePage() ?>', pageNumber: '<?php echo $this->getPageNumber() ?>', pageAjax: onSet, fieldAjax: field, dataAjax: <?php echo $data; ?>, headAjax: $.parseJSON('<?php echo simec_json_encode($this->getHead()); ?>')
                        <!--                    , enableCount : --><?php //echo "{$this->_enableCount()}"; ?><!--')-->
                    };
                    console.info(data);

//                $.post(window.location.href , data , function(html){
//                    element.closest('.container-listing').find('tbody:first').hide().html(html).fadeIn();
//                });

                });
            </script>
        <?php endif;
    }

    private function typePageScroll()
    {
        ?>
        <div class="col-lg-12 text-center" style="padding-bottom: 20px;">
            Carregando...
        </div>
    <?php
    }

    public function page()
    {
        $actions = $this->getActions();

//        $this->setPage($this->getPage() + 1);
        $data = $this->getData();
        if ($data):
            ?>
            <?php $n = $this->getOffset(); ?>
            <?php foreach ($data as $dataValue): ?>
            <tr>
                <td><?php echo $n ?></td>
                <?php $this->renderActionList($dataValue) ?>
                <?php foreach ($dataValue as $key => $value) : ?>
                    <td><?php echo $value ?></td>

                <?php endforeach; ?>
            </tr>
            <?php $n++; ?>
        <?php endforeach; ?>
        <?php endif;


    }
}
