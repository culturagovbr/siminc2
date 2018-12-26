<?php
/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_View
 * @subpackage Helper
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @version    $Id: FormSelect.php 24594 2012-01-05 21:27:01Z matthew $
 */


/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


/**
 * Helper to generate "select" list of options
 *
 * @category   Zend
 * @package    Zend_View
 * @subpackage Helper
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
class Simec_View_Helper_Select extends Simec_View_Helper_Element
{
    public function select($name, $label = null, $value = null, $options = array(), $attribs = array(), $config = array())
    {
        $attribs = (array) $attribs;

        foreach($attribs as $chave => $attrib){
            if(is_numeric($chave)){
                $attribs[$attrib] = $attrib;
                unset($attribs[$chave]);
            } elseif ('placeholder' == $chave){
                $attribs['data-placeholder'] = $attrib;
            }
        }
        
        unset($attribs['']);

        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $attribs['class'] = isset($attribs['class']) ? 'form-control chosen-select' .  $attribs['class'] : 'form-control chosen-select';
        $attribs['data-placeholder'] = isset($attribs['data-placeholder']) ? $attribs['data-placeholder'] : 'Selecione';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;
        
        $config['visible'] = isset($config['visible']) ? $config['visible'] : true;
        $complemento = empty($config['texto-padrao']) ? array('' => '') : $config['texto-padrao'];
        $options = $complemento + $options;

        $podeEditar = isset($config['pode-editar']) ? $config['pode-editar'] : true;
        if(!$podeEditar || $podeEditar==='N'){

            $marcados = (array) $value;
            $texto = array();
            foreach($marcados as $marcado){
                $texto[] = $options[$marcado];
            }
            $xhtml = '<p class="form-control-static" id="' . $id . '">' . implode("<br />", $texto) . '</p>';
        } else {
            $xhtml = $this->buildSelect($name, $value, $attribs, $options);
        }
        return $this->buildField($xhtml, $label, $attribs, $config);

    }

    /**
     * Generates 'select' list of options.
     *
     * @access public
     *
     * @param string|array $name If a string, the element name.  If an
     * array, all other parameters are ignored, and the array elements
     * are extracted in place of added parameters.
     *
     * @param mixed $value The option value to mark as 'selected'; if an
     * array, will mark all values in the array as 'selected' (used for
     * multiple-select elements).
     *
     * @param array|string $attribs Attributes added to the 'select' tag.
     *
     * @param array $options An array of key-value pairs where the array
     * key is the radio value, and the array value is the radio text.
     *
     * @param string $listsep When disabled, use this list separator string
     * between list values.
     *
     * @return string The select tag and options XHTML.
     */
    public function buildSelect($name, $value = null, $attribs = null, $options = null, $config = array())
    {
        $info = $this->_getInfo($name, $value, $attribs, $options);
        $help = isset($attribs['help']) ? true : false;
        extract($info); // name, id, value, attribs, options, listsep, disable
        
        // force $value to array so we can compare multiple values to multiple
        // options; also ensure it's a string for comparison purposes.
        $value = array_map('strval', (array) $value);

        // check if element may have multiple values
        $multiple = '';

        if (substr($name, -2) == '[]') {
            // multiple implied by the name
            $multiple = ' multiple="multiple"';
        }

        if (isset($attribs['multiple'])) {
            // Attribute set
            if ($attribs['multiple']) {
                // True attribute; set multiple attribute
                $multiple = ' multiple="multiple"';

                // Make sure name indicates multiple values are allowed
                if (!empty($multiple) && (substr($name, -2) != '[]')) {
                    $name .= '[]';
                }
            } else {
                // False attribute; ensure attribute not set
                $multiple = '';
            }
            unset($attribs['multiple']);
        }

        // now start building the XHTML.
        $disabled = '';
        if (true === $disable) {
            $disabled = ' disabled="disabled"';
        }

        if ($help) {
        	$help = "<span class='help-block m-b-none'><i class='fa fa-question-circle' style='color: #1c84c6;'></i> {$attribs['help']}</span>";
        }
        
        // Build the surrounding select element first.
        $xhtml = '<select'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . $multiple
                . $disabled
                . $this->_htmlAttribs($attribs)
                . ">\n    ";

        // build the list of options
        $list       = array();
        $translator = $this->getTranslator();
        foreach ((array) $options as $opt_value => $opt_label) {
            if (is_array($opt_label)) {
                $opt_disable = '';
                if (is_array($disable) && in_array($opt_value, $disable)) {
                    $opt_disable = ' disabled="disabled"';
                }
                if (null !== $translator) {
                    $opt_value = $translator->translate($opt_value);
                }
                $opt_id = ' id="' . $this->view->escape($id) . '-optgroup-'
                        . $this->view->escape($opt_value) . '"';
                $list[] = '<optgroup'
                        . $opt_disable
                        . $opt_id
                        . ' label="' . $this->view->escape($opt_value) .'">';
                foreach ($opt_label as $val => $lab) {
                    $list[] = $this->_build($val, $lab, $value, $disable);
                }
                $list[] = '</optgroup>';
            } else {
                $list[] = $this->_build($opt_value, $opt_label, $value, $disable);
            }
        }

        // add the options to the xhtml and close the select
        $xhtml .= implode("\n    ", $list) . "\n</select>";

        $xhtml .= $help;
        
        return $xhtml;
    }

    /**
     * Builds the actual <option> tag
     *
     * @param string $value Options Value
     * @param string $label Options Label
     * @param array  $selected The option value(s) to mark as 'selected'
     * @param array|bool $disable Whether the select is disabled, or individual options are
     * @return string Option Tag XHTML
     */
    protected function _build($value, $label, $selected, $disable)
    {
        if (is_bool($disable)) {
            $disable = array();
        }
        /*
         * Por favor corriga esta caquinha
         */
        //$this->view->setEscape(function ($dado){ return htmlspecialchars($dado, ENT_COMPAT, 'iso-8859-1'); });
        $opt = '<option'
             . ' value="' . ($value) . '"'
             . ' label="' . ($label) . '"';

        // selected?
        if (in_array((string) $value, $selected)) {
            $opt .= ' selected="selected"';
        }

        // disabled?
        if (in_array($value, $disable)) {
            $opt .= ' disabled="disabled"';
        }

        $opt .= '>' . ($label) . "</option>";

        return $opt;
    }

}
