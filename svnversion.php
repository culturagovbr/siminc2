<?php
$svninfo = `svn info`;
preg_match('/Revision: ([0-9]+)/', $svninfo, $result);
$strRev = explode(': ', $result[1]);
echo( 'Revis�o: ' . end($strRev) );

