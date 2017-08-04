"""Class for building & running DCJ test executables."""


class Tester(object):
  """A class for building & running DCJ test executables.
  """

  def __init__(self, builder, runner):
    self.builder = builder
    self.runner = runner

  def AddToParser(self, parser):
    self.builder.AddToParser(parser)
    self.runner.AddToParser(parser)
    parser.add_argument('--executable',
                        help='path of the executable to be built. By default '
                        'it\'s the same as path of source with removed '
                        'filename extension.')
    return parser

  def Run(self, args):
    self.builder.Run(args)
    args['executable'] = self.builder.ExecutablePath(args)
    self.runner.Run(args)

  def Description(self):
    return 'Builds and locally runs a solution.'
