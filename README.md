[![Travis](https://travis-ci.org/fathomssen/qtredmine.svg?branch=master)](https://travis-ci.org/fathomssen/qtredmine)

Redmine REST API Qt client
==========================

This is a [Redmine](http://redmine.org) client C++ library, written in Qt. It uses Redmine's REST API to
access a Redmine instance.

This library requires Qt version 5.2+.

Features
--------
* Both login/password and API key authentication
* Simple methods to get serveral resources (issues, projects etc.) from a Redmine instance
* Simple methods to update serveral resources (issues and time entries at the moment) in a Redmine instance

To use Redmine custom fields with qtredmine, please install the `redmine_shared_api` plugin from
https://github.com/anovitsky/redmine_shared_api.

Documentation
-------------
Please have a look at the Doxygen documentation at
https://fathomssen.github.io/qtredmine.

Example
-------
Please have a look at the examples in the Doxygen documentation at
https://fathomssen.github.io/qtredmine/examples.html.

Furthermore, you can find a real-world usage at https://github.com/fathomssen/redtimer.

Todo
----
* Complete the simple API to get more resource types
