"""CLI for local testing of solutions in Distributed Code Jam."""
import argparse
import os
from os import chmod
import stat
import subprocess
import sys

from dcj import build
from dcj import command_chooser
from dcj import configuration
from dcj import run
from dcj import test


def _print(x):
  print ' '.join(x)
  return 0  # Tell tool that command execution was succesfull.


def _subprocess_call_with_error_catching(command):
  try:
    subprocess.call(command)
    return 0
  except OSError as e:
    if e.args == (2, 'No such file or directory'):
      raise ValueError('Command {0} not found.'.format(command[0]))
    else:
      raise ValueError(
          'Error when executing command {0!r}: {1!r}.'.format(command, e))


def _create_script(script_path, content):
  with open(script_path, 'w') as f:
    f.write(content)
  chmod(script_path, stat.S_IRWXU | stat.S_IROTH | stat.S_IXOTH)


if __name__ == '__main__':
  parser = argparse.ArgumentParser(prog='dcj')
  config = configuration.Configuration()
  # TODO(jbartosik): allow using different configs.
  config.Load(
      os.path.join(os.path.dirname(os.path.realpath(__file__)), 'config.json'))
  builder = build.Build(config)
  runner = run.Run(config)
  tester = test.Tester(builder, runner)
  chooser = command_chooser.CommandChooser({
      'build': builder,
      'run': runner,
      'test': tester,
  })
  chooser.AddToParser(parser)
  parser.add_argument(
      '--dry-run',
      action='store_true',
      help='Only print commands, don\'t execute them.',
      default=False)
  args = parser.parse_args()

  if args.dry_run:
    builder.SetCommandExecutor(_print)
    runner.SetCommandExecutor(_print)
  else:
    builder.SetCommandExecutor(_subprocess_call_with_error_catching)
    builder.SetScriptCreator(_create_script)
    runner.SetCommandExecutor(_subprocess_call_with_error_catching)

  try:
    chooser.Run(args)
  except (NotImplementedError, RuntimeError, ValueError) as error:
    print error
    sys.exit(1)
