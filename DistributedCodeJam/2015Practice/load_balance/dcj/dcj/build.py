"""Class for building DCJ test executables."""
from os import path


class Build(object):
  """A class for building DCJ test executables."""

  def __init__(self, config):
    self._command_executor = lambda x: None
    self._script_creator = lambda x, y: None
    self._config = config

  def SetCommandExecutor(self, command_executor):
    self._command_executor = command_executor

  def SetScriptCreator(self, script_creator):
    self._script_creator = script_creator

  def AddToParser(self, parser):
    """Adds flags to parser and returns it."""
    parser.add_argument('--source', required=True,
                        help='source file of the solution.')
    parser.add_argument('--language',
                        help='language of the solution. Valid choices are: '
                        '{0}. If the flag is not provided language will be '
                        'deduced from source file extensions.'
                        .format(
                            ', '.join(self._SUPPORTED_LANGUAGE_TO_EXTENSION))
                       )
    parser.add_argument('--library',
                        help='source file of library generating input.')
    parser.add_argument('--executable',
                        help='path of the executable to be built. By default '
                        'it\'s the same as path of source with removed '
                        'filename extension.')
    parser.add_argument('--extra_flags',
                        help='comma-separated list of additional flags to pass '
                        'to compiler. For '
                        'example --extra_flags="-Wall,-Wextra".')
    return parser

  def Run(self, args):
    # TODO(jabrtosik): When running python tell user / check if file meets
    # necessary conditions:
    # * Is executable.
    # * First line is #!/path/to/interpreter.
    self._ValidateArgs(args)
    source_extension = self._SourceExtension(args)
    commands_builder = self._BUILDER_FOR_EXTENSION[source_extension]
    for command in commands_builder(self, args):
      if self._command_executor(command) != 0:
        raise RuntimeError('Build failed.')

  def Description(self):
    return 'Builds solution for local testing.'

  def _ValidateArgs(self, args):
    """Validate arguments.

    Args:
      args: arguments to be validated.

    Raises:
      ValueError: exception with string describing the problem detected.
    """
    if 'language' in args and args['language']:
      if args['language'] not in self._SUPPORTED_LANGUAGE_TO_EXTENSION:
        raise ValueError('--language  must be one of {0!r} but it was {1!r}.'
                         .format(self._SUPPORTED_LANGUAGE_TO_EXTENSION.keys(),
                                 args['language']))
      else:
        # Skip file extension validations.
        return

    source_extension = self._SourceExtension(args)

    if source_extension not in self._BUILDER_FOR_EXTENSION:
      raise ValueError('Source extension must be one of {0!r} but it was {1!r}.'
                       .format(self._BUILDER_FOR_EXTENSION.keys(),
                               source_extension))

    if self._HasLibrary(args):
      library_extension = self._LibraryExtension(args)

      if source_extension == '.c':
        if library_extension != '.c' and library_extension != '.h':
          raise ValueError('C solutions should have a .h or .c library')
      elif source_extension == '.cc' or source_extension == '.cpp':
        if (library_extension != '.cc' and library_extension != '.cpp' and
            library_extension != '.c' and library_extension != '.h'):
          raise ValueError('C++ solutions should have a .cc/.cpp or .h library')
      elif source_extension == '.py':
        if library_extension != '.py':
          raise ValueError('Python solutions should have a .py library')
      elif source_extension == '.java':
        if library_extension != '.java':
          raise ValueError('Java solutions should have a .java library')

  def _CBuildCommands(self, args):
    """Prepare commands to build solution written in C.

    Args:
      args: arguments of the build.

    Returns:
      tuple in which each item is a tuple with command that will execute a step
      of building solution.
    """
    compiler = self._config.GetStringConfigValue('c-compiler')
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    include_dir = path.join(dcj_root, 'includes')
    local_zeus_path = path.join(dcj_root, 'libraries', 'zeus_local.c')
    message_path = path.join(dcj_root, 'libraries', 'message_internal.c')
    # TODO(jbartosik): support compilers that don't have -I flag for include
    # dirs.
    compiler_args = (
        self._config.GetStringListConfigValue('c-compiler-flags') + [
            '-I' + include_dir,
            local_zeus_path, message_path,
        ]
    )
    if self._HasLibrary(args):
      compiler_args += [args['library']]
    build_solution_command = (
        (compiler,) + tuple(compiler_args) + self.ExtraFlags(args) +
        (args['source'], '-o', self.ExecutablePath(args),)
    )

    return (build_solution_command,)

  def _CcBuildCommands(self, args):
    """Prepare commands to build solution written in C++.

    Args:
      args: arguments of the build.

    Returns:
      tuple in which each item is a tuple with command that will execute a step
      of building solution.
    """
    # TODO(jbartosik): support other compilers.
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    include_dir = path.join(dcj_root, 'includes')
    c_compiler_with_flags = tuple(
        [self._config.GetStringConfigValue('c-compiler')] +
        self._config.GetStringListConfigValue('c-compiler-flags') +
        ['-I' + include_dir]
    )
    cpp_compiler_with_flags = tuple(
        [self._config.GetStringConfigValue('cpp-compiler')] +
        self._config.GetStringListConfigValue('cpp-compiler-flags') +
        ['-I' + include_dir]
    )
    c_sources = (path.join(dcj_root, 'libraries', 'zeus_local.c'),
                 path.join(dcj_root, 'libraries', 'message_internal.c'),
                )
    c_object_files_build_commands = tuple(
        c_compiler_with_flags +
        (source_file, '-c', '-o', path.splitext(source_file)[0] + '.o')
        for source_file in c_sources
    )
    object_files = tuple(
        path.splitext(source_file)[0] + '.o' for source_file in c_sources
    )

    files = [args['source']]
    if self._HasLibrary(args):
      files += [args['library']]

    build_solution_command = (
        cpp_compiler_with_flags + self.ExtraFlags(args) + object_files +
        tuple(files) + ('-o', self.ExecutablePath(args),)
    )

    return c_object_files_build_commands + (build_solution_command,)

  def _BuildPythonObjectFileCommand(self, c_source, output):
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    return (
        'gcc', '-c', '-fpic',
        # TODO(jbartosik): Don't rely on users having this exact version of
        # python.
        '-I/usr/include/python2.7',
        '-I' + path.join(dcj_root, 'includes'),
        path.join(dcj_root, 'libraries', c_source),
        '-o', path.join(dcj_root, 'libraries', output),
    )

  def _BuildJavaObjectFileCommand(self, c_source, output):
    """Return command building .c file to work with Java via SWIG."""
    compiler = self._config.GetStringConfigValue('c-compiler')
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    include_dir = path.join(dcj_root, 'includes')
    # TODO(jbartosik): support compilers that don't have -I flag for include
    # dirs.
    compiler_args = (
        '-c', '-fpic',
        '-I' + include_dir,
    )
    java_include_options = tuple(
        [
            '-I' + directory
            for directory in
            self._config.GetDirListConfigValue('java-include-dirs')
        ]
    )
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    return (compiler,) + compiler_args + java_include_options + (
        path.join(dcj_root, 'libraries', c_source),
        '-o', path.join(dcj_root, 'libraries', output),
    )

  def _PyBuildCommands(self, unused_args):
    """Returns tuple with commands for building Python solutions."""
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')

    # TODO(jbartosik): use another directory to store object files.
    build_object_file_commands = tuple(
        self._BuildPythonObjectFileCommand(item + '.c', item + '.o')
        for item in self._MESSAGE_SO_PYTHON_INGREDIENTS)

    object_files = tuple(
        path.join(dcj_root, 'libraries', item + '.o')
        for item in self._MESSAGE_SO_PYTHON_INGREDIENTS
    )

    link_object_files = (
        ('ld', '-shared',) +
        object_files +
        ('-o', path.join(dcj_root, 'libraries', '_message.so',))
    )
    return build_object_file_commands + (link_object_files,)

  def _JavaBuildCommands(self, args):
    """Prepare commands to build solution written in Java.

    Args:
      args: arguments of the build.

    Returns:
      tuple in which each item is a tuple with command that will execute a step
      of building solution.
    """
    # Prepare a script that will run java solution. This step is needed because
    # parunner works only with single executable files.
    solution_class_dir = path.dirname(path.realpath(args['source']))
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    message_dir = path.join(dcj_root, 'libraries')
    library_dir = path.dirname(path.realpath(args['library']))
    classpath = ':'.join((message_dir, library_dir,))
    self._script_creator(
        self.ExecutablePath(args),
        self._config.GetStringConfigValue('java-wrapper-file-content').format(
            solution_class_dir,
            classpath,
        )
    )

    # Build message_.o and libmessage.so
    # TODO(jbartosik): deduplicate with Python
    build_object_file_commands = tuple(
        self._BuildJavaObjectFileCommand(item + '.c', item + '.o')
        for item in self._MESSAGE_SO_JAVA_INGREDIENTS)

    object_files = [
        path.join(dcj_root, 'libraries', item + '.o')
        for item in self._MESSAGE_SO_JAVA_INGREDIENTS]

    link_object_files = tuple(
        [self._config.GetStringConfigValue('java-native-library-linker')] +
        self._config.GetStringListConfigValue(
            'java-native-library-linker-options') +
        object_files +
        ['-o', path.join(
            dcj_root, 'libraries',
            self._config.GetStringConfigValue('java-native-library-name'),
            )
        ]
    )

    # Create a class file to be ran.
    build_class_file_command = (
        self._config.GetStringConfigValue('java-compiler'),
        path.join(dcj_root, 'libraries', 'Wrapper.java'),
        args['source'],
        path.join(dcj_root, 'libraries', 'message.java'),
        path.join(dcj_root, 'libraries', 'messageJNI.java'),
        self._config.GetStringConfigValue('java-compiler-classpath-arg'),
        classpath,
    )
    return (
        build_object_file_commands +
        (link_object_files, build_class_file_command,)
    )

  def _SourceExtension(self, args):
    if 'language' in args and args['language']:
      return self._SUPPORTED_LANGUAGE_TO_EXTENSION[args['language']]
    return path.splitext(args['source'])[1]

  def _LibraryExtension(self, args):
    return path.splitext(args['library'])[1]

  def ExtraFlags(self, args):
    if 'extra_flags' in args and args['extra_flags']:
      return tuple(args['extra_flags'].split(','))
    return ()

  def ExecutablePath(self, args):
    if args['executable']:
      return args['executable']
    if self._SourceExtension(args) == '.py':
      return args['source']
    return path.splitext(args['source'])[0]

  def _HasLibrary(self, args):
    return 'library' in args and args['library'] is not None

  _BUILDER_FOR_EXTENSION = {
      '.c': _CBuildCommands,
      '.cc': _CcBuildCommands,
      '.cpp': _CcBuildCommands,
      '.java': _JavaBuildCommands,
      '.py': _PyBuildCommands,
  }

  _MESSAGE_SO_PYTHON_INGREDIENTS = (
      'message_internal',
      'message_wrap_python',
      'zeus_local',
  )

  _MESSAGE_SO_JAVA_INGREDIENTS = (
      'message_internal',
      'message_wrap_java',
      'zeus_local',
  )

  _SUPPORTED_LANGUAGE_TO_EXTENSION = {
      'C': '.c',
      'C++': '.cc',
      'Java': '.java',
      'Python': '.py',
  }
