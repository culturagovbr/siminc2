
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
  <div style="width:100%;text-align:right">
    <img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
  </div>
  <div style="padding:5px;text-align:justify;">
    <table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <tr>
        <td align="center" bgcolor="#CCCCCC" colspan="2">Usuário</td>
    </tr>
      <tr>
        <td width="30%" class="SubtituloDireita">Usuário:</td>
          <td><?php echo campo_texto("wsusuario","S","S","Usuário","21","","","","","","","id='wsusuario'", '', WEB_SERVICE_SIOP_USUARIO); ?></td>
      </tr>
    <tr>
      <td class="SubtituloDireita">Senha:</td>
      <td>
        <input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);"
               onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();"
               onmouseover="MouseOver(this);" value="<?php echo WEB_SERVICE_SIOP_SENHA; ?>" size="22" id="wssenha" name="wssenha">
        <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
      </td>
    </tr>
    <tr>
        <td align="center" bgcolor="#CCCCCC" colspan="2">WebService</td>
    </tr>
    <tr>
        <td class="SubtituloDireita">Versão:</td>
        <td>
            <input type="radio" name="webservice" id="webservice_2" value="2" /><label for="webservice_2">Antigo</label>
            <input type="radio" name="webservice" id="webservice_1" value="1" checked /><label for="webservice_1">Novo</label>
        </td>
    </tr>
<!--    <tr>
        <td class="SubtituloDireita">Ambiente:</td>
        <td>
            <input type="radio" name="ambiente" id="ambiente_1" value="1"
                <?php // echo((('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin']))?'':'checked'); ?> />
            <label for="ambiente_1">Produção</label>
            <input type="radio" name="ambiente" id="ambiente_2" value="2"
                <?php // echo((('simec_desenvolvimento' == $_SESSION['baselogin']) || ('simec_espelho_producao' == $_SESSION['baselogin']))?'checked':''); ?> />
            <label for="ambiente_2">Desenvolvimento</label>
        </td>
    </tr>-->
    <tr>
      <td align="center" bgcolor="#D5D5D5" colspan="2">
        <input type="button" name="btn_enviar" onclick="enviaSolicitacao();" value="ok" />
        <input type="button" name="btn_cancelar"  value="cancelar"
               onclick="document.getElementById('<?php echo $id; ?>').style.display='none';" />
      </td>
    </tr>
    </table>
  </div>
</div>