<?php

class Ted_Form_Justificativa extends Ted_Form_Abstract
{			
	public function init()
	{
		parent::init();			
		$this->setName('justificativa')
		->setAttrib('id', 'form-justificativa')
		->setMethod('post')
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');		
		$tcpid->setAttribs(array('id'=>'tcpid'));
		
		$justid = new Zend_Form_Element_Hidden('justid');
		$justid->setAttrib('id', 'justid');

        $tipoemenda = new Zend_Form_Element_Radio('tipoemenda');
        $tipoemenda->addMultiOption('S', 'Sim');
        $tipoemenda->addMultiOption('N', 'N�o');
        $tipoemenda->setValue('N');
        $tipoemenda->setSeparator('&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;');
        $tipoemenda->addErrorMessage('Valor � necess�rio e n�o pode ser vazio');
        $tipoemenda->setRequired(true);

		$identificacao = new Zend_Form_Element_Textarea('identificacao');
		$identificacao->setRequired(TRUE);
		$identificacao->addErrorMessage('Valor � necess�rio e n�o pode ser vazio');
		$identificacao->setAttribs(array(
            'id' => 'identificacao',
            'class' => 'form-control',
            'maxlength' => '70',
            'rows' => '3',
            'required' => ''
        ));
		
		$objetivo = new Zend_Form_Element_Textarea('objetivo');
		$objetivo->setRequired(TRUE);
		$objetivo->addErrorMessage('Valor � necess�rio e n�o pode ser vazio');
		$objetivo->setAttribs(array(
            'id' => 'objetivo',
            'class' => 'form-control',
            'maxlength' => '490',
            'rows' => '3',
            'data-toggle' => 'popover',
            'data-delay' => 'show:200,hide:100',
            'data-title' => 'Atentar para observa��o presente neste quadro.',
            'data-trigger' => 'hover',
            'data-container' => 'body',
            'data-placement' => 'bottom',
            'data-content' => 'Favor Preencher com a descri��o do objeto a ser executado, indicando, inclusive, o campus em que se localizar� o objeto. O objeto � o que deve ser fisicamente entregue � sociedade ao final da execu��o do Plano de Trabalho.',
            'required' => ''
        ));
		
		$ugRepassadora = new Zend_Form_Element_Text('ugrepassadora');
		$ugRepassadora->setAttribs(array(
            'id' => 'ugrepassadora',
            'class' => 'form-control',
            'disabled' => ''
        ));
		
		$ugRecebedora  = new Zend_Form_Element_Text('ugrecebedora');
		$ugRecebedora->setAttribs(array(
            'id' => 'ugrecebedora',
            'class' => 'form-control',
            'disabled' => ''
        ));
		
		$justificativa = new Zend_Form_Element_Textarea('justificativa');
		$justificativa->setRequired(TRUE);
		$justificativa->addErrorMessage('Valor � necess�rio e n�o pode ser vazio');
		$justificativa->setAttribs(array(
            'id' => 'justificativa',
            'class' => 'form-control',
            'maxlength' => '350',
            'rows' => '3',
            'data-toggle' => 'popover',
            'data-delay' => 'show:200,hide:100',
            'data-title' => 'Atentar para observa��o presente neste quadro.',
            'data-trigger' => 'hover',
            'data-container' => 'body',
            'data-placement' => 'bottom',
            'data-content' => 'Favor registrar: Contextualiza��o da obra no campus em que o projeto ser� executado; Motiva��o da obra, isto �, qual o problema que a obra busca sanar e qual a demanda para o projeto. Caso a proposta tenha recursos a serem descentralizados em mais de um exerc�cio, o proponente dever� inserir no campo da justificativa o coment�rio de como o recurso dever� ser distribu�do ao longo dos exerc�cios. Ex.: A constru��o em quest�o dever� ter aporte de recursos distribu�dos em mais de um exerc�cio. Sendo a parcela para 2013 de R$ XX, para 2014 de R$ YY e para 2015 de R$ ZZ.',
            'required' => ''
        ));
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'justificativa-form.php'))));
		$this->addElements(array($tcpid,$justid,$tipoemenda,$identificacao,$objetivo,$ugRecebedora,$ugRepassadora,$justificativa));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
	}

}