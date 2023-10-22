describe 'database' do
  def run_script(commands)
    raw_output = nil
    IO.popen("./db", "r+") do |pipe|
      commands.each do |command|
        pipe.puts command
      end

      pipe.close_write

      raw_output = pipe.gets(nil)

    end
    raw_output.split("\n")
  end

  it 'inserts and retrieves a row' do
    result = run_script([
      "insert 1 user1 person1@example.com",
      "select",
      ".exit"
    ])
    expect(result).to match_array([
      "db > Executed.",
      "db > (1, user1, person1@example.com)",
      "db > ",
    ])
  end

  it 'test .exit command' do
    result = run_script([
      ".exit"
    ])

    expect(result).to match_array([
      "db > "
    ])
  end

  it 'test unimplemented command' do
    result = run_script([
      ".table",
      ".exit"
    ])

    expect(result).to match_array([
      "db > Unrecognized command .table.",
      "db > "
    ])
  end

  it 'test select statement' do
    result = run_script([
      "select (a,b)",
      ".exit"
    ])

    expect(result).to match_array([
      "db > Select Statement.",
      "db > "
    ])
  end

  it 'test uppercase select statement' do
    result = run_script([
      "SELECT (a,b)",
      ".exit"
    ])

    expect(result).to match_array([
      "db > Select Statement.",
      "db > "
    ])
  end

  it 'test insert statement' do
    result = run_script([
      "insert (a,b)",
      ".exit"
    ])

    expect(result).to match_array([
      "db > Insert Statement.",
      "db > "
    ])
  end

  it 'test uppercase insert statement' do
    result = run_script([
      "INSERT (a,b)",
      ".exit"
    ])

    expect(result).to match_array([
      "db > Insert Statement.",
      "db > "
    ])
  end
end
