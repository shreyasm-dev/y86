const name = Deno.args[0];

if (!name) {
  console.error("a name must be provided");
  Deno.exit(1);
}

await Deno.writeTextFile(
  `./simulator/test/${name}`,
  await Deno.readTextFile(`./assembler/test/${name}-expected`)
);
