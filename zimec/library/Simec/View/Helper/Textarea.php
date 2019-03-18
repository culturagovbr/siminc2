<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_Textarea extends Simec_View_Helper_Element
{
    public function textarea($name, $label = null, $value = null, $attribs = array(), $config = array())
    {
        $attribs = (array) $attribs;

        foreach($attribs as $chave => $attrib){
            if(is_numeric($chave)){
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            }
        }

        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'text';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : 'form-control';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        $value = simec_stripslashes($value);
        // Construindo o input

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){
            $xhtml = '<p class="form-control-static" id="' . $id . '">' . nl2br($value) . '</p>';
        } else {
            $xhtml = '<textarea'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' class="' . $class . '"'
                . $this->_htmlAttribs($attribs)
                . ">$value</textarea>";
        }

        return $this->buildField($xhtml, $label, $attribs, $config);
    }
}
