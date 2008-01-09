CREATE TABLE accounts (
   accname varchar(50) NOT NULL,
   banned tinyint(4) DEFAULT '0',
   onlyload tinyint(4) DEFAULT '0',
   adminrights int(11) DEFAULT '0',
   adminlevel int(11) DEFAULT '0',
   lastused timestamp(14) DEFAULT '0',
   comments text DEFAULT '' NOT NULL,
   adminip varchar(16) DEFAULT '0.0.0.0',
   PRIMARY KEY (accname)
);

CREATE TABLE profiles(
   accname varchar(50) NOT NULL,
   realname varchar(100) DEFAULT '(unknown)' NOT NULL,
   age int(11) DEFAULT '0' NOT NULL,
   sex varchar(10) DEFAULT '-' NOT NULL,
   country varchar(30) DEFAULT '' NOT NULL,
   icq varchar(20) DEFAULT '' NOT NULL,
   email varchar(250) DEFAULT '' NOT NULL,
   webpage varchar(250) DEFAULT '' NOT NULL,
   favhangout varchar(100) DEFAULT '' NOT NULL,
   favquote varchar(250) DEFAULT '' NOT NULL,
   PRIMARY KEY (accname)
);


CREATE TABLE main (
   accname varchar(50) NOT NULL,
   nickname varchar(50) DEFAULT 'unknown' NOT NULL,
   x float(10,2) DEFAULT '30.50' NOT NULL,
   y float(10,2) DEFAULT '30.00' NOT NULL,
   level varchar(80) DEFAULT 'onlinestartlocal.graal' NOT NULL,
   maxhp int(11) DEFAULT '6' NOT NULL,
   hp int(11) DEFAULT '6' NOT NULL,
   rupees int(11) DEFAULT '0' NOT NULL,
   arrows int(11) DEFAULT '5' NOT NULL,
   bombs int(11) DEFAULT '10' NOT NULL,
   glovepower int(11) DEFAULT '1' NOT NULL,
   swordpower int(11) DEFAULT '1' NOT NULL,
   shieldpower int(11) DEFAULT '1' NOT NULL,
   headimg varchar(50) DEFAULT 'head0.gif' NOT NULL,
   bodyimg varchar(50) DEFAULT 'body.png' NOT NULL,
   swordimg varchar(50) DEFAULT 'sword1.gif' NOT NULL,
   shieldimg varchar(50) DEFAULT 'shield1.gif' NOT NULL,
   colors varchar(8) DEFAULT 'cakessss' NOT NULL,
   sprite tinyint(4) DEFAULT '2' NOT NULL,
   status tinyint(4) DEFAULT '16' NOT NULL,
   horseimg varchar(50) DEFAULT '' NOT NULL,
   horsebushes tinyint(4) DEFAULT '0' NOT NULL,
   magic tinyint(4) DEFAULT '0' NOT NULL,
   kills int(11) DEFAULT '0' NOT NULL,
   deaths int(11) DEFAULT '0' NOT NULL,
   onlinetime bigint(20) DEFAULT '0' NOT NULL,
   lastip bigint(20) DEFAULT '0' NOT NULL,
   alignment tinyint(4) DEFAULT '100' NOT NULL,
   weapons text DEFAULT '' NOT NULL,
   chests text DEFAULT '' NOT NULL,
   flags text DEFAULT '' NOT NULL,
   apcounter int(11) DEFAULT '0' NOT NULL,
   PRIMARY KEY (accname)
);
