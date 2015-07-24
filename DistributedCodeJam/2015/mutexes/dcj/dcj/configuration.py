"""Class for getting configuration and nicely reporting problems (if any)."""

import json
from os import path


class Configuration(object):
  """Class for getting configuration and nicely reporting problems (if any)."""

  def __init__(self):
    self._parsed_config = {}
    self._config_path = ''

  def Load(self, config_path):
    """Load configuration from specified file.

    The file should be readable, it should be a properly formated JSON and it
    should contain a single Object.

    Args:
      config_path: path to file containg configuration.

    Raises:
      RuntimeError: containig details of the problem.
    """
    try:
      config = open(config_path, 'r')
    except IOError as e:
      raise RuntimeError('Opening configuration file {0} failed with: {1!r}'
                         .format(config_path, e))

    try:
      parsed_config = json.load(config)
    except ValueError as e:
      raise RuntimeError(
          'Couldn\'t parse configuration file(as JSON): {0!r}'.format(e))

    if not isinstance(parsed_config, dict):
      raise RuntimeError(
          'Config file {0} parsed successfully as JSON. Expected content was a '
          'single Object, found: {1!r}.'.format(config_path, parsed_config))
    self._parsed_config = parsed_config
    self._config_path = config_path

  def _RaiseConfigurationFileError(self, key, extra_message):
    raise RuntimeError('Error in configuration file {0} in key {1}: {2}'
                       .format(self._config_path, key, extra_message))

  def _GetRawConfigValue(self, key):
    if unicode(key) not in self._parsed_config:
      self._RaiseConfigurationFileError(key, 'key not found')
    return self._parsed_config[unicode(key)]

  def GetStringConfigValue(self, key):
    value = self._GetRawConfigValue(key)
    if not isinstance(value, (str, unicode,)):
      self._RaiseConfigurationFileError(
          key, 'expected value to be a string but it is {0!r}.'.format(value))
    return value

  def GetStringListConfigValue(self, key):
    """Returns value for the key if it exists and is a list of strings."""
    value = self._GetRawConfigValue(key)
    if not isinstance(value, (list)):
      self._RaiseConfigurationFileError(
          key, 'expected value to be a list but it is {0!r}.'.format(value))
    for item in value:
      if not isinstance(item, (str, unicode,)):
        self._RaiseConfigurationFileError(
            key, 'expected all items of the list to be strings but one of them '
            'is {0!r}.'.format(item))
    return value

  def GetExistingFilePath(self, key):
    value = self.GetStringConfigValue(key)
    if not path.isfile(value):
      self._RaiseConfigurationFileError(
          key, 'expected value to point to an existing file, file {1!r} does '
          'not exist. '.format())
    return value

  def GetDirListConfigValue(self, key):
    value = self.GetStringListConfigValue(key)
    for item in value:
      if not path.isdir(item):
        self._RaiseConfigurationFileError(
            key, 'expected value to point to an existing directory, directory '
            '{0!r} does not exist.'.format(item))
    return value
