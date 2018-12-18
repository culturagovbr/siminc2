<?php
$_SESSION['sislayoutbootstrap'] = 'zimec';
include APPRAIZ . "includes/cabecalho.inc";
echo '<br>';


define ('CLASSES_MODELO', APPRAIZ . 'aspar/classes/model/');
set_include_path(CLASSES_MODELO . PATH_SEPARATOR . get_include_path());
$dir = opendir(realpath(APPRAIZ . 'aspar/classes/model/'));
if ($dir) {
    $count = 0;
    while (($item = readdir($dir)) !== false) {
        $classe = explode(".class", $item);


        if ($classe[0] != "." && $classe[0] != ".." && $classe[0] != ".svn") {
            //ver($classe,d);
            inspecionarClasse($classe[0]);
            $reflector = new ReflectionClass("Aspar_Model_" . $classe[0]);
            $class_methods = $reflector->getMethods(ReflectionMethod::IS_PUBLIC);


            foreach ($class_methods as $key => $item) {
                //  ver($item->name,d);

                $dados[$count][$key]["classe"] = $item->class;
                $dados[$count][$key]["metodo"] = $item->name;
                $dados[$count][$key]['comentario'] = $reflector->getMethod($item->name)->getDocComment();
            }


        }
        $count++;

    }


    closedir($dir);

}
function inspecionarClasse($class)
{
    //$class = "Acao";
    if (PHP_OS != "WINNT") { // Se "não for Windows"
        $separaDiretorio = ":";
        $include_path = get_include_path();
        $include_path_tokens = explode($separaDiretorio, $include_path);
    } else { // Se for Windows
        $raiz = strtolower(substr(APPRAIZ, 0, 2));
        $separaDiretorio = ";$raiz";
        $include_path = get_include_path();
        $include_path = str_replace('.;', $raiz, strtolower($include_path));
        $include_path = str_replace('/', '\\', $include_path);
        $include_path_tokens = explode($separaDiretorio, $include_path);
        $include_path_tokens = str_replace("//", "/", $include_path_tokens);
        $include_path_tokens [0] = str_replace($raiz, '', $include_path_tokens [0]);
    }

    foreach ($include_path_tokens as $prefix) {
        // Recupera a última posição do array, substituindo o array_pop para parar o erro de parâmetro por referência
        $aClasse = explode('_', $class);
        $file = $aClasse[(count($aClasse) - 1)];

        $pathModule = $prefix . $file . '.php';
        if (file_exists($pathModule))
            require_once $pathModule;
        $path [0] = $prefix . $class . '.class.inc';
        $path [1] = $prefix . $class . '.php';


        foreach ($path as $thisPath) {
            if (file_exists($thisPath)) {
                require_once $thisPath;
                return;
            }
        }
    }
}

$_SESSION['sislayoutbootstrap'] = 'false';


?>


<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5> Validação de código </h5>

            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>
        <div class="ibox-content">

            <table class="table table-striped table-bordered dataTable">
                <thead>
                <tr>
                    <th>Classe</th>
                    <th>Método</th>
                    <th>Comentário</th>

                </tr>
                </thead>
                <tbody>
                <?php
                if($dados):
                    foreach ($dados as $classes) {
                        foreach ($classes as $metodo) {

                            ?>
                            <tr>
                                <td><?php echo $metodo['classe']; ?></td>
                                <td><?php echo $metodo['metodo']; ?></td>
                                <td style="overflow: hidden;max-width: 500px;"><?php echo "<pre>" . $metodo['comentario'] . "</pre>"; ?></td>
                            </tr>

                        <?php } ?>
                    <?php } ?>
                <?php endif; ?>
                </tbody>
            </table>

        </div>
    </div>
</div>


<link rel='stylesheet' type='text/css' href='../par/css/jquery.dataTables.min.css'>
<script type="text/javascript" src="../par/js/jquery.dataTables.js" charset="utf8"></script>
<script>
    $('.dataTable').dataTable();
</script>