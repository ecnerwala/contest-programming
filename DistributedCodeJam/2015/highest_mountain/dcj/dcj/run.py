"""Class for running DCJ test executables."""
import os

from os import path

# First item in the array is default.
_OUTPUT_MODES = ['tagged', 'all', 'files']


class Run(object):
  """A class for running DCJ test executables.
  """

  def __init__(self, config):
    self._command_executor = lambda x: None
    self._config = config

  def _AppendToEnvPath(self, path_variable, value_to_append):
    if path_variable not in os.environ:
      os.environ[path_variable] = ''
    os.environ[path_variable] = (
        ':'.join((os.environ[path_variable], value_to_append)))

  def SetCommandExecutor(self, command_executor):
    self._command_executor = command_executor

  def AddToParser(self, parser):
    """Adds flags to parser and returns it."""
    parser.add_argument('--executable', help='path of executable to run.')
    parser.add_argument('--nodes', required=True, type=int,
                        help='number of nodes that will run the solution.')
    output = parser.add_argument(
        '--output',
        default=_OUTPUT_MODES[0]
    )
    # TOOD(jbartosik): Add line breaks.
    output.help = """Mode of output. Allowed values are:
    *tagged*::: This is default. In this mode each row of stdout from each
    instance of the solution will be prepended with:
    STDOUT ${ROW_NUMBER}:
    and sent to stdout of the command. Stderr will be treated similarily.
    *all*::: In this mode stdout and stderr from all machines will be sent to
    (respectively) stdout and stderr of the command.
    *files*::: In this mode each row of stdout from each instance of the
    solution will written to file
    ${EXECUTABLE_FILE_NAME}.stdout.${MACHINE_NUMBER}
    Stderr will be treated similarily.
    """
    return parser

  def Run(self, args):
    """Actually run the required executable."""
    self._ValidateArgs(args)
    parunner = path.join(
        os.path.dirname(os.path.realpath(__file__)), '..', 'executable',
        self._config.GetStringConfigValue('parunner-file'))
    dcj_root = path.join(path.dirname(path.realpath(__file__)), '..')
    # TODO(onufry): This modifies the user's env directly, it would be better to
    # just pass a modified map of env vars over to the command executor.
    self._AppendToEnvPath('PYTHONPATH', path.join(dcj_root, 'libraries'))
    self._AppendToEnvPath('PYTHONPATH', path.join(dcj_root, 'modules'))
    self._AppendToEnvPath('PATH', '.')
    os.environ['LD_LIBRARY_PATH'] = path.join(dcj_root, 'libraries')
    if 0 != self._command_executor((parunner,
                                    '--n', str(args['nodes']),
                                    '--stdout', args['output'],
                                    '--stderr', args['output'],
                                    args['executable'])):
      raise RuntimeError('Run failed.')

  def Description(self):
    return 'Runs previously built solution locally.'

  def _ValidateArgs(self, args):
    """Validate arguments.

    Args:
      args: arguments to be validated.

    Raises:
      ValueError: exception with string describing the problem detected.
    """
    if args['nodes'] <= 0:
      raise ValueError('argument --nodes must be positive.')
    if args['output'] not in _OUTPUT_MODES:
      raise ValueError(
          'argument --output must be one of ' + ', '.join(_OUTPUT_MODES))
