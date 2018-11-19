<?php
/**
 * Classe de mapeamento da entidade wssof.ws_acoesdto
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-06-2017
 * @link     no link
 */

require_once(APPRAIZ . 'spo/ws/sof/Qualitativo.php');
require_once APPRAIZ .'includes/classes/Modelo.class.inc';

/**
 * Wssof_Ws_acoesdto
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Wssof_Importador extends Modelo
{
    public function importarTabelasSof($exercicio, $dadosImportar, $momento = null)
    {
        $aTabelasCarga = [
            'basesGeograficasDTO' => 'Wssof_Ws_BasesGeograficasDto',
            'esferasDTO' => 'Wssof_Ws_EsferasDto',
            'funcoesDTO' => 'Wssof_Ws_FuncoesDto',
            'macroDesafiosDTO' => 'Wssof_Ws_MacroDesafiosDto',
            'momentosDTO' => 'Wssof_Ws_MomentosDto',
            'perfisDTO' => 'Wssof_Ws_PerfisDto',
            'periodicidadesDTO' => 'Wssof_Ws_PeriodicidadesDto',
            'produtosDTO' => 'Wssof_Ws_ProdutosDto',
            'regioesDTO' => 'Wssof_Ws_RegioesDto',
            'subFuncoesDTO' => 'Wssof_Ws_SubFuncoesDto',
            'tiposAcaoDTO' => 'Wssof_Ws_TiposAcaoDto',
            'tiposInclusaoDTO' => 'Wssof_Ws_TiposInclusaoDto',
            'tiposProgramaDTO' => 'Wssof_Ws_TiposProgramaDto',
            'unidadesMedidaDTO' => 'Wssof_Ws_UnidadesMedidaDto',
            'unidadesMedidaIndicadorDTO' => 'Wssof_Ws_UnidadesMedidaIndicadorDto',

            'agendasSamDTO' => 'Wssof_Ws_AgendaSamDto',
            'orgaosDTO' => 'Wssof_Ws_OrgaosDto',
            'acoesDTO' => 'Wssof_Ws_AcoesDto',
            'localizadoresDTO' => 'Wssof_Ws_LocalizadoresDto',
            'planosOrcamentariosDTO' => 'Wssof_Ws_PlanosOrcamentariosDto',

            'unidadesOrcamentariasDto' => 'Wssof_Ws_UnidadesOrcamentariasDto',
            'programasDto' => 'Wssof_Ws_ProgramasDto',
            'indicadoresDto' => 'Wssof_Ws_IndicadoresDto',
            'objetivosDto' => 'Wssof_Ws_ObjetivosDto',
        ];

        foreach($dadosImportar->return as $tabela => $aRegistros){
            if(empty($aRegistros)){
                continue;
            }

            if(key_exists($tabela, $aTabelasCarga)){

                $arquivo = str_replace('Wssof_', '', $aTabelasCarga[$tabela]);
                if(file_exists(APPRAIZ . "wssof/classes/{$arquivo}.inc")){
                    include_once APPRAIZ . "wssof/classes/{$arquivo}.inc";
                }

                // Excluindo registros antigos
                $model = new $aTabelasCarga[$tabela];
                switch ($tabela) {
                    case 'acoesDTO':
                    case 'localizadoresDTO':
                    case 'planosOrcamentariosDTO':
                        $model->excluirVarios("exercicio = '{$exercicio}' and codigomomento = '{$momento}'");
                        break;
                    case 'orgaosDTO':
                    case 'unidadesOrcamentariasDto':
                    case 'programasDto':
                    case 'indicadoresDto':
                        $model->excluirVarios("exercicio = '{$exercicio}'");
                        break;
                    default:
                        $model->excluirVarios(" true ");
                        break;
                }
                unset($model);

                foreach($aRegistros as $count => $dados){
                    $model = new $aTabelasCarga[$tabela];
                    $model->realizarCarga($dados, TRUE);
                    unset($model);
                }
            }
        }
        $this->commit();
    }

    public function obterProgramacaoCompleta($exercicio, $momento, $map = null){
        $servico = new SiopSoapCurl_Quantitativo_Service_ProgramacaoCompleta();
        $servico->getXml()
            ->setExercicio($exercicio)
            ->setCodigoMomento($momento)
            ->setListasRetornadas(array(
                'retornarAcoes',
                'retornarLocalizadores',
                'retornarPlanosOrcamentarios'
            ))
        ;
        $dadosImportar = $servico->configure()->request();
        $this->importarTabelasSof($exercicio, $dadosImportar, $momento);
    }
}//end Class
?>