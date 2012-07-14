# Contactology for Drupal Messaging

Adds a new send method to the Drupal 6 [Messaging framework](http://drupal.org/project/messaging) to send emails through a [Contactology](http://www.contactology.com/) transactional campaign. The benefit over the simple email method is that Contactology automatically provides tracking of open and clickthrough rates.

(c) 2012, Morris DigitalWorks

## Installation

The modules requires the Contactology API wrapper for PHP. [Download it from Contactology](http://www.contactology.com/email-marketing-api/wrappers), rename it to "class.Contactology.php" (that is, change the extension), and add it to the `messaging_contactology` directory. Note that the wrapper requires your PHP to be built with JSON support and the cURL extension. 

This module is designed for the 4.x branch of the Drupal 6 Messaging package and has been tested against 6.x-4.0-beta8. It almost certainly does not work with 6.x-2.4 or with Drupal 7.

After installing the Messaging package, enable the core `messaging` module and the `messaging_mail` module. You can disable the Simple Mail send method if you don't want to use it, but the module must be enabled. Enable the `messaging_contactology` module.

To access Contactology, the module needs an API key. Login to your Contactology account, select "API Keys" from the Settings menu, and generate a new key. On your Drupal dashboard, go to Messaging settings, click the Send Method tab, then the Contactology tab (or go to `/admin/messaging/settings/method/contactology`). Paste the key from Contactology into the "API key" box and save the form.

## Transactional email campaign

You will also need to setup a Contactology campaign for tracking sent messages. Most campaigns are used for broadcasting, where a single message is sent to multiple recipients. However the Messaging framework is meant for one-to-one communication. Contactology provides a transactional campaign type that can handle this use, but that option is only exposed through the API. Instead of going to their site, you'll need to create a transactional campaign by clicking the "Create new campaign" link in Drupal on the same page where you pasted in the API key.

Transactional campaigns are a powerful but obscure feature of Contactology and come with a few caveats. Although they are a separate type of campaign, they're not mentioned in the Contactology interface and don't get their own tab alongside "Draft", "In Progress", "Recurring", etc. The campaign you create through the Drupal interface will appear under the "Complete" tab. This is important to keep in mind if you ever go on a cleaning spree and start deleting completed campaigns.

Also, because of the individual nature of a transactional campaign, Contactology does not save or archive the messages. (Or at least it did not in mid-2011 when this module was written.) For this reason the module does not expose the "view in browser" and "show in archive" options and always turns them off when creating a transactional campaign. This affects other functionality, most importantly the "forward to a friend" feature, which by default gets a link in the email footer added by Contactology.