<?php
/**
 * @file
 * Configuration options for the Contactology integration module.
 * 
 */

/**
 * Generate settings form for Contactology send method.
 *
 * @param $form_state 
 *
 * @return mixed
 */
function messaging_contactology_admin_settings($form_state) {
  $current_key = variable_get('messaging_contactology_api_key', '');
  $current_campaign = variable_get('messaging_contactology_campaign', '');
  $campaign_status = $current_campaign ?
    t('Contactology campaign for sending messages has been configured.') :
    t('No Contactology campaign found.');
  $form['messaging_contactology_api_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#default_value' => $current_key,
    '#description' => t("Generate an API key from the Settings menu in Contactology and paste it here."),
  );
  $form['messaging_contactology_contact_source'] = array(
    '#title' => t('Contact source'),
    '#type' => 'textfield',
    '#default_value' => messaging_contactology_contact_source(),
    '#description' => t("Description that Contactology uses to explain why a user is receiving e-mails. Defaults to the site name."),
  );
  $form['messaging_contactology_campaign_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contactology campaign'),
    '#description' => t('Before messages can be sent through Contactology, a special transactional campaign has to be created. This cannot be done through the Contactology interface, so it must be done on this screen.'),
  );
  $form['messaging_contactology_campaign_settings']['status'] = array(
    '#value' => "<p>" . $campaign_status . ' ' . l(t('Create new campaign'), 'admin/messaging/settings/method/contactology/create') . "</p>",
  );
  $form['messaging_contactology_campaign_settings']['messaging_contactology_campaign'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign ID'),
    '#default_value' => $current_campaign,
    '#size' => 5,
  );
  return system_settings_form($form);
}

/**
 * Generate form for creating a Contactology campaign.
 *
 * @param $form_state
 *
 * @return mixed
 */
function messaging_contactology_admin_campaign_form($form_state) {
  global $user;

  // Make sure the API key for Contactology has been configured, so the 
  // create call can be made.
  if (!messaging_contactology_api_key()) {
    drupal_set_message(t('Cannot create campaign until Contactology API key is configured.'), 'error');
    return;
  }

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Campaign information')
  );
  $form['info']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Campaign Name'),
    '#description' => t('The name that will appear on this campaign in the Contactology interface.'),
    '#required' => TRUE,
  );
  $form['info']['sender_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender address'),
    '#description' => t('The e-mail address that messages sent through Contactology will appear to come from.'),
    '#default_value' => variable_get('messaging_mail_returnpath', variable_get('site_mail', ini_get('sendmail_from'))),
    '#required' => TRUE,
  );
  $form['info']['sender_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender name'),
    '#description' => t('The name that messages sent through Contactology will appear to come from.'),
    '#default_value' => variable_get('site_name', 'Drupal'),
    '#required' => TRUE,
  );
  $form['test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test message'),
    '#description' => t('Contactology will send a test message immediately after the campaign the campaign is created. This is required by Contactology.'),
  );
  $form['test']['test_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient address'),
    '#description' => t('The e-mail address to send the test message to.'),
    '#default_value' => $user->mail,
    '#required' => TRUE,
  );
  $form['test']['test_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#description' => t('The line to use for the subject of the test message.'),
    '#default_value' => t('Test message'),
    '#required' => TRUE,
  );
  $form['test']['test_msg_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Text body'),
    '#default_value' => t("Hello.\n\nThis is a test message from !site_name", array('!site_name' => variable_get('site_name', 'Drupal'))),
    '#required' => TRUE,
  );
  $form['test']['test_msg_html'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML body'),
    '#default_value' => t("<p>Hello.</p>\n<p>This is a test message from !site_name</p>", array('!site_name' => variable_get('site_name', 'Drupal'))),
    '#required' => TRUE,
  );
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => 'Optional settings',
  );
  $form['options']['replyto_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to address'),
    '#description' => t('Leave blank to use the sender address from above.'),
  );
  $form['options']['replyto_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to name'),
    '#description' => t('Leave blank to use the sender name from above.'),
  );
  $form['options']['track_opens'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track opens'),
    '#default_value' => 1,
  );
  $form['options']['track_html_clicks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track HTML clicks'),
    '#default_value' => 1,
  );
  $form['options']['track_text_clicks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track plain text clicks'),
    '#default_value' => 1,
  );
  $form['options']['track_replies'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track replies'),
  );
  $form['options']['authenticate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Authenticate messages'),
    '#default_value' => 0,
  );
  $form['options']['google_analytics'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Analytics tracking'),
    '#description' => t('If Google Analytics has been configured on this site, messages sent through the Messaging framework via Contactology can be tracked by entering a campaign label here.'),
    '#size' => 20,
  );
  /* Not offering, because these doesn't make sense for the messaging interface.
  $form['options']['show_in_archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Archive this e-mail'),
  );
  $form['options']['view_in_browser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "view in browser" link'),
  );
  */
  /* Also not offered: Twitter integration, ClickTale integration */
  $form['submit'] = array('#type' => 'submit', '#value' => t('Create'));
  return $form;
}

/**
 * Validate the create campaign form to ensure the addresses are valid.
 */
function messaging_contactology_admin_campaign_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['sender_email'])) {
    form_set_error('sender_email', t('Sender address is not a valid e-mail address.'));
  }
  if (!valid_email_address($form_state['values']['test_address'])) {
    form_set_error('test_address', t('Test address is not a valid e-mail address.'));
  }
  if ($form_state['values']['replyto_address'] && !valid_email_address($form_state['values']['replyto_address'])) {
    form_set_error('replyto_address', t('Reply-to address is not a valid e-mail address.'));
  }
}

/**
 * Create a new campaign via a Contactology API call.
 */
function messaging_contactology_admin_campaign_form_submit($form, &$form_state) {
  // Pull in the API wrapper provided by Contactology
  require_once(drupal_get_path('module', 'messaging_contactology') . '/class.Contactology.php');
  // Make sure the API key for Contactology has been configured.
  $key = messaging_contactology_api_key();
  if (!$key) {
    drupal_set_message(t('Cannot create campaign until Contactology API key is configured.'), 'error');
    return;
  }

  // Assemble list of optional parameters for the API call.
  $options = array(
    'authenticate' => $form_state['values']['authenticate'] ? TRUE : FALSE,
    'trackReplies' => $form_state['values']['track_replies'] ? TRUE : FALSE,
    'trackOpens' => $form_state['values']['track_opens'] ? TRUE : FALSE,
    'trackClickThruHTML' => $form_state['values']['track_html_clicks'] ? TRUE : FALSE,
    'trackClickThruText' => $form_state['values']['track_text_clicks'] ? TRUE : FALSE,
    /* Options forced to off because they're not offerend in the form. */
    'showInArchive' => FALSE,
    'viewInBrowser' => FALSE,
    'automaticTweet' => FALSE,
    // 'recipientName'    // @todo Not sure if we need this yet.
    /* Unsupported fields:
    'clickTaleName'
    'clickTaleCustomeFields'*/
  );
  // Contactology complains if a blank argument is passed in, even if it's 
  // optional, so only add these to the list if they were defined.
  if ($form_state['values']['replyto_address']) {
    $options['replyToEmail'] = $form_state['values']['replyto_address'];
  }
  if ($form_state['values']['replyto_name']) {
    $options['replyToName'] = $form_state['values']['replyto_name'];
  }
  if ($form_state['values']['google_analytics']) {
    $options['googleAnalyticsName'] = $form_state['values']['google_analytics'];
  }

  // Create connection Contactology and make API call to create the campaign.
  $c = new Contactology($key);
  try {
    $campaign_id = $c->Campaign_Create_Transactional(
      // 1: testContact
      array(
        'email' => $form_state['values']['test_address'],
      ),
      // 2: testReplacements
      array(
        'msg_subject' => $form_state['values']['test_subject'],
        'msg_body_text' => $form_state['values']['test_msg_text'],
        'msg_body_html' => $form_state['values']['test_msg_html'],
      ),
      // 3: campaignName
      $form_state['values']['name'],
      // 4: subject
      '{msg_subject}',
      // 5: senderEmail
      $form_state['values']['sender_email'],
      // 6: senderName
      $form_state['values']['sender_name'],
      // 7: content
      array(
        'text' => "{msg_body_text}",
        'html' => "{msg_body_html}",
      ),
      // 8: optionalParameters
      $options
    );
  } catch (Exception $e) {
    form_set_error(NULL, t("Couldn't create campaign (Contactology error %code)", array('%code' => $e->getCode())));
    form_set_error(NULL, $e->getMessage());
    return;
  }
  variable_set('messaging_contactology_campaign', $campaign_id);
  drupal_set_message(t("Created campaign with ID %id", array('%id' => $campaign_id)));
  $form_state['redirect'] = 'admin/messaging/settings/method/contactology';
  return $form;
}

?>
