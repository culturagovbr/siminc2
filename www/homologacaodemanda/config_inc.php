<?php
# Mantis - a php based bugtracking system

# Copyright (C) 2000 - 2002  Kenzaburo Ito - kenito@300baud.org
# Copyright (C) 2002 - 2007  Mantis Team   - mantisbt-dev@lists.sourceforge.net

# Mantis is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Mantis is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Mantis.  If not, see <http://www.gnu.org/licenses/>.

	# --------------------------------------------------------
	# $Id: config_inc.php.sample,v 1.19.2.2 2007-10-25 17:13:42 giallu Exp $
	# --------------------------------------------------------

	# This sample file contains the essential files that you MUST
	# configure to your specific settings.  You may override settings
	# from config_defaults_inc.php by assigning new values in this file

	# Rename this file to config_inc.php after configuration.

	###########################################################################
	# CONFIGURATION VARIABLES
	###########################################################################

	# In general the value OFF means the feature is disabled and ON means the
	# feature is enabled.  Any other cases will have an explanation.

	# Look in http://www.mantisbt.org/manual or config_defaults_inc.php for more
	# detailed comments.

	# --- database variables ---------

	# set these values to match your setup
	$g_hostname      = "";
	$g_db_username   = "";
	$g_db_password   = "";
	$g_database_name = "";
	$g_db_type       = "pgsql";

	# --- email variables -------------
	$g_administrator_email  = 'thiagotasca@gmail.com';
	$g_webmaster_email      = 'thiagotasca@gmail.com';

	# the "From: " field in emails
	$g_from_email           = 'thiagotasca@gmail.com';

	# the return address for bounced mail
	$g_return_path_email    = 'thiagotasca@gmail.com';

	# --- file upload settings --------
	# This is the master setting to disable *all* file uploading functionality
	#
	# The default value is ON but you must make sure file uploading is enabled
	#  in PHP as well.  You may need to add "file_uploads = TRUE" to your php.ini.
	$g_allow_file_upload	= ON;
?>
