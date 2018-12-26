<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Element extends Zend_View_Helper_FormElement
{

    protected function buildField($xhtml, $label, $attribs = array(), $config = array())
    {
        $icon = !empty($config['icon']) ? '<span class="input-group-addon"><span class="' . $config['icon'] . '"></span></span></span>' : null;
        $help = !empty($config['help']) ? '<span class="input-group-addon help-tooltip" data-toggle="tooltip" data-placement="left" title="' . $config['help'] . '"><span class="glyphicon glyphicon-question-sign"></span></span>' : null;
        $date = !empty($config['date']) ? " {$config['date']} " : '';
        $labelSize = !empty($config['label-size']) ? $config['label-size'] : 3;
        $inputSize = !empty($config['input-size']) ? $config['input-size'] : (12 - $labelSize);
        if ($attribs['type']=='password'){
            $passtype = '<div title="Clique aqui para visualizar a senha." class="fa link fa-eye eye-slash col-sm-1 col-md-1 col-lg-1" id="eye-slash'.$attribs['id'].'" style="font-size:3em; color: #1c84c6" data-target="#'.$attribs['id'].'"></div>';
            $passtype .= '<script>';
            $passtype .= '   $("#eye-slash'.$attribs['id'].'").click(function () {
                                var id = $(this).data("target");
                                if($(id).attr("type") == "text"){
                                    $(id).attr("type", "password");
                                    $(this).removeClass("fa-eye-slash");
                                    $(this).addClass("fa-eye");
                                } else{
                                    $(id).attr("type", "text");
                                    $(this).removeClass("fa-eye");
                                    $(this).addClass("fa-eye-slash");
                                }
                              });';
            $passtype .= '</script>';
            $inputSize--;
        }
        $labelErros = (is_array($config['errorValidate']) && count($config['errorValidate'])) ?  '<label id="-error" class="error">' .implode('</label><br /><label id="-error" class="error">', $config['errorValidate']) . '</label>' : '';
        $classErro  = $labelErros ? ' has-error ' : '';
        $classGroup = '';
        
        if ($icon || $help) {
            $xhtml = '
                <div class="input-group">
                    ' . $icon . '
                    ' . $xhtml . '
                    ' . $help . '
                </div>
            ';
        }

        if(isset($config['formTipo']) && $config['formTipo'] == Simec_View_Helper::K_FORM_TIPO_VERTICAL){
            $classLabel = '';
            $classInput = ' ' . $date;
        } else if ($labelSize || $inputSize) {
        	$classLabel = "col-sm-{$labelSize} col-md-{$labelSize} col-lg-{$labelSize} control-label";
        	$classInput = "col-sm-{$inputSize} col-md-{$inputSize} col-lg-{$inputSize} " . $date;
        } else {
            $classLabel = 'col-sm-3 col-md-3 col-lg-3 control-label';
            $classInput = 'col-sm-9 col-md-9 col-lg-9 ' . $date;
        }
        
        if (isset($config['visible']) && $config['visible'] == false) {
        	$classGroup.= ' hidden ';
       	}


        if ($label) {
            $required = is_array($attribs) && in_array('required', $attribs) ? '<span class="campo-obrigatorio" title="Campo obrigatório">*</span>' : '';

            $titleOfLabel = $attribs['title']? 'title="'. $attribs['title']. '"': NULL;
            return '               
                <style>
                    .dropdown-menu{
                        z-index: 10000 !important;
                    }
                </style>

                <div class="form-group ' . $classErro . $classGroup . '">
                    <label for="' . $config['label-for'] . '" class="' . $classLabel . '" '. $titleOfLabel. '>' . $label . ': ' . $required . '</label>
                    <div class="' . $classInput . '">
                        '. $xhtml .'
                        '. $labelErros .'
                    </div>
                    '.$passtype.'
                    <div style="clear:both"></div>
                </div>
            ';
        }else{
            return '               
                <style>
                    .dropdown-menu{
                        z-index: 10000 !important;
                    }
                </style>

                <div class="form-group ' . $classErro . $classGroup . '">
                    <div class="' . $classInput . '">
                        '. $xhtml .'
                        '. $labelErros .'
                    </div>
                </div>
            ';
        }
    }
}
