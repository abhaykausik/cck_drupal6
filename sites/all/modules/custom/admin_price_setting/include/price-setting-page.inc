<?php
// $Id$

/**
* This file is created to show all links of admin price setting
*/

function custom_price_setting_page()
{
	$price_list_menu = array();

	$price_list_menu['price-settings/church-business-sponsor']     = 'Church Business Sponsor Price settings';
	$price_list_menu['price-settings/non-church-business-sponsor'] = 'Non Church Business Sponsor Price settings';
	$price_list_menu['price-settings/church-personal-sponsor']     = 'Church Personal Sponsor Price settings';
	$price_list_menu['price-settings/non-church-personal-sponsor'] = 'Non Church Personal Sponsor Price settings';
	$price_list_menu['price-settings/group-sponsor']               = 'Group Sponsor Price settings';
	$price_list_menu['price-settings/workconnection']              = 'Work Connection Sponsor Price settings';

	return theme('price_setting_page', $price_list_menu);
}

