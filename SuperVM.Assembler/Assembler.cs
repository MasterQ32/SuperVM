﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace SuperVM.Assembler
{
	public static class Assembler
	{
		static Regex annotationMatcher = new Regex(@"\[\s*(.*?)\s*\]", RegexOptions.Compiled);
		static Regex labelMatcher = new Regex(@"^(\w+):\s*(.*)\s*$", RegexOptions.Compiled);
		static Regex instructionMatcher = new Regex(@"(\w+)(?:\s+([@-]?\w+|'.'))?", RegexOptions.Compiled);

		public static VMAssembly Assemble(string src)
		{
			var sourceLines = src.Split(new[] { '\n' });
			var patches = new Dictionary<int, string>();
			var labels = new Dictionary<string, int>();

			var code = new List<ulong>();
			var instructions = new List<Instruction>();
			var lines = new List<int>();
			var source = new List<string>();
			for (int i = 0; i < sourceLines.Length; i++)
			{
				var line = sourceLines[i].Trim();
				{ // Process comments
					var idx = line.IndexOf(';');
					if (idx >= 0)
						line = line.Substring(0, idx);
				}

				var uncommented = line;

				{ // Process labels
					var match = labelMatcher.Match(line);
					if (match.Success)
					{
						var label = match.Groups[1].Value;
						labels.Add(label, code.Count);
						line = match.Groups[2].Value;
					}
				}

				if (string.IsNullOrWhiteSpace(line))
					continue;

				var annotations = new HashSet<string>();
				line = annotationMatcher.Replace(line, (m) =>
				{
					annotations.Add(m.Groups[1].Value.ToLower());
					return "";
				});
				line = line.Trim();

				{
					var match = instructionMatcher.Match(line);
					if (match.Success == false)
						throw new InvalidOperationException("Invalid instruction: " + line);

					var mnemonic = match.Groups[1].Value;

					uint argument = 0;
					if (match.Groups[2].Length > 0)
					{
						var argstring = match.Groups[2].Value;
						if (argstring.StartsWith("@"))
						{
							// Add patch note for labels.
							patches.Add(code.Count, argstring.Substring(1));
						}
						else if (argstring.StartsWith("'"))
						{
							argument = (uint)argstring[1];
						}
						else if (argstring.StartsWith("0x"))
						{
							argument = Convert.ToUInt32(argstring.Substring(2), 16);
						}
						else if (argstring.StartsWith("0b"))
						{
							argument = Convert.ToUInt32(argstring.Substring(2), 10);
						}
						else if (argstring.StartsWith("0d"))
						{
							argument = Convert.ToUInt32(argstring.Substring(2), 10);
						}
						else
						{
							if (argstring.StartsWith("-"))
							{
								unchecked
								{
									argument = (uint)Convert.ToInt32(argstring, 10); ;
								}
							}
							else
								argument = Convert.ToUInt32(argstring, 10);
						}
					}

					if (mnemonics.ContainsKey(mnemonic) == false)
					{
						throw new InvalidOperationException("Unknown mnemonic: " + mnemonic);
					}

					var instruction = mnemonics[mnemonic];

					foreach (var annotation in annotations)
					{
						if (annotation.StartsWith("ci:"))
						{
							instruction.CommandInfo = UInt16.Parse(annotation.Substring(3));
							continue;
						}

						if (annotation.StartsWith("cmd:"))
						{
							instruction.Command = (Command)Enum.Parse(typeof(Command), annotation.Substring(4));
							continue;
						}

						switch (annotation)
						{
							case "f:yes":
							instruction.ModifyFlags = true;
							break;
							case "f:no":
							instruction.ModifyFlags = false;
							break;
							case "r:discard":
							instruction.Output = OutputType.Discard;
							break;
							case "r:push":
							instruction.Output = OutputType.Push;
							break;
							case "r:jump":
							instruction.Output = OutputType.Jump;
							break;
							case "i0:zero":
							instruction.Input0 = InputType.Zero;
							break;
							case "i0:pop":
							instruction.Input0 = InputType.Pop;
							break;
							case "i0:peek":
							instruction.Input0 = InputType.Peek;
							break;
							case "i0:arg":
							instruction.Input0 = InputType.Argument;
							break;
							case "i1:zero":
							instruction.Input1 = InputType.Zero;
							break;
							case "i1:pop":
							instruction.Input1 = InputType.Pop;
							break;
							case "ex(z)=x":
							instruction.ExecutionZ = ExecutionMode.Always;
							break;
							case "ex(z)=0":
							instruction.ExecutionZ = ExecutionMode.Zero;
							break;
							case "ex(z)=1":
							instruction.ExecutionZ = ExecutionMode.One;
							break;
							case "ex(n)=x":
							instruction.ExecutionN = ExecutionMode.Always;
							break;
							case "ex(n)=0":
							instruction.ExecutionN = ExecutionMode.Zero;
							break;
							case "ex(n)=1":
							instruction.ExecutionN = ExecutionMode.One;
							break;
							default:
							throw new InvalidOperationException("Unrecognized annotation: " + annotation);
						}
					}

					ulong encoded = 0;

					encoded |= ((uint)(instruction.ExecutionZ) << 0);
					encoded |= ((uint)(instruction.ExecutionN) << 2);
					encoded |= ((uint)(instruction.Input0) << 4);
					encoded |= ((uint)(instruction.Input1) << 6);
					encoded |= ((uint)(instruction.Command) << 7);
					encoded |= ((uint)(instruction.CommandInfo) << 13);
					encoded |= ((uint)(instruction.ModifyFlags ? 1 : 0) << 29);
					encoded |= ((uint)(instruction.Output) << 30);
					encoded |= ((ulong)argument << 32);

					code.Add(encoded);
					instructions.Add(instruction);
					lines.Add(i + 1);
					source.Add(uncommented);
				}
			}

			{ // Install patches
				foreach (var patch in patches)
				{
					var target = patch.Value;
					var position = labels[target];
					code[patch.Key] =
						(code[patch.Key] & 0xFFFFFFFF) |
						((ulong)position << 32);
				}
			}

			return new VMAssembly(code.ToArray(), instructions.ToArray(), lines.ToArray(), source.ToArray());
		}


		static Dictionary<string, Instruction> mnemonics = new Dictionary<string, Instruction>()
		{
			{ "nop", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Zero, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "push", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Argument, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "drop", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "dup", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Peek, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "jmp", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Argument, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Jump, Argument = 0,  } },
			{ "jmpi", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Jump, Argument = 0,  } },
			{ "ret", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.Copy, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Jump, Argument = 0,  } },
			{ "load", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Argument, Input1 = InputType.Zero, Command = Command.Load, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "loadi", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.Load, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "store", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Argument, Input1 = InputType.Pop, Command = Command.Store, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "storei", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Store, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "get", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Argument, Input1 = InputType.Zero, Command = Command.Get, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "geti", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.Get, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "set", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Argument, Input1 = InputType.Pop, Command = Command.Set, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "seti", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Set, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "bpget", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Zero, Input1 = InputType.Zero, Command = Command.BpGet, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "bpset", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.BpSet, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "spget", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Zero, Input1 = InputType.Zero, Command = Command.SpGet, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "spset", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.SpSet, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "cpget", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Zero, Input1 = InputType.Zero, Command = Command.CpGet, CommandInfo = 1, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "add", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "sub", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 1, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "cmp", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 1, ModifyFlags = true, Output = OutputType.Discard, Argument = 0,  } },
			{ "mul", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 2, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "div", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 3, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "mod", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 4, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "and", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 5, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "or", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 6, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "xor", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 7, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "not", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Zero, Command = Command.Math, CommandInfo = 8, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "rol", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 9, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "ror", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 10, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "asl", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 11, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "asr", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 12, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "shl", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 13, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "shr", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Pop, Input1 = InputType.Pop, Command = Command.Math, CommandInfo = 14, ModifyFlags = false, Output = OutputType.Push, Argument = 0,  } },
			{ "syscall", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Zero, Input1 = InputType.Zero, Command = Command.SysCall, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
			{ "hwio", new Instruction() { ExecutionZ = ExecutionMode.Always, ExecutionN = ExecutionMode.Always, Input0 = InputType.Zero, Input1 = InputType.Zero, Command = Command.HwIO, CommandInfo = 0, ModifyFlags = false, Output = OutputType.Discard, Argument = 0,  } },
		};
	}
}
