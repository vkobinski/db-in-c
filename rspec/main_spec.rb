describe 'database' do
  before do
    `rm -rf dbfile`
    `touch dbfile`
  end

  after do
    `rm -rf dbfile`
    `touch dbfile`
  end

  def run_script(commands)
    raw_output = nil
    IO.popen('./db dbfile', 'r+') do |pipe|
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
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'insert (user1,person1@example.com)',
                          'select',
                          '.exit'
                        ])
    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > (1, user1, person1@example.com)',
                                    'db > '
                                  ])
  end

  it 'create table with no name' do
    result = run_script([
                          'CREATE TABLE (nome:text)',
                          '.exit'
                        ])

    expect(result).to match_array([
                                    'db > Table name field not populated.',
                                    'db > '
                                  ])
  end

  it 'create table with no field' do
    result = run_script([
                          'CREATE TABLE test',
                          '.exit'
                        ])

    expect(result).to match_array([
                                    "db > Table can't be created with no fields.",
                                    'db > '
                                  ])
  end

  it 'test invalid command' do
    result = run_script([
                          'inser',
                          '.exit'
                        ])
    expect(result).to match_array([
                                    'db > Invalid command.',
                                    'db > '
                                  ])
  end

  it 'test create table' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, endereco: text, idade: int)',
                          '.exit'
                        ])

    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > '
                                  ])
  end

  it 'inserts row with no fields' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'insert',
                          '.exit'
                        ])
    expect(result).to match_array([
                                    'db > Executed.',
                                    "db > Record can't be saved with no fields.",
                                    'db > '
                                  ])
  end

  it 'create table exits and check .info' do
    _result = run_script([
                           'CREATE TABLE pessoa (name: text, email: text)',
                           '.exit'
                         ])

    result2 = run_script([
                           '.info',
                           '.exit'
                         ])

    expect(result2).to match_array([
                                     'db > Table pessoa:',
                                     "\tColumn Name: id, Id",
                                     "\tColumn Name: name, Text",
                                     "\tColumn Name: email, Text",
                                     'db > '
                                   ])
  end

  it 'inserts and retrieves a row with space between commas' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'insert (  user1  ,  person1@example.com  )',
                          'select',
                          '.exit'
                        ])
    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > (1, user1, person1@example.com)',
                                    'db > '
                                  ])
  end

  it 'inserts 2 rows and retrieves 2 rows' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'insert (user1,person1@example.com)',
                          'insert (user2,person2@example.com)',
                          'select',
                          '.exit'
                        ])
    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > (1, user1, person1@example.com)',
                                    '(2, user2, person2@example.com)',
                                    'db > '
                                  ])
  end

  it 'test .exit command' do
    result = run_script([
                          '.exit'
                        ])

    expect(result).to match_array([
                                    'db > '
                                  ])
  end

  it 'test unimplemented command' do
    result = run_script([
                          '.table',
                          '.exit'
                        ])

    expect(result).to match_array([
                                    'db > Unrecognized command .table.',
                                    'db > '
                                  ])
  end

  it 'test insert statement' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'insert (a,b)',
                          '.exit'
                        ])

    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > '
                                  ])
  end

  it 'test uppercase insert statement' do
    result = run_script([
                          'CREATE TABLE ab (a:text, b:text)',
                          'INSERT (a,b)',
                          '.exit'
                        ])

    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > '
                                  ])
  end

  it 'test insert into database, and select after loading from the disk' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'INSERT (a,b)',
                          '.exit'
                        ])

    result2 = run_script([
                           'select',
                           '.exit'
                         ])

    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > '
                                  ])

    expect(result2).to match_array([
                                     'db > (1, a, b)',
                                     'db > '
                                   ])
  end

  it 'inserts 2 rows and retrieves 2 rows after saving' do
    result = run_script([
                          'CREATE TABLE pessoa (nome: text, email: text)',
                          'insert (user1,person1@example.com)',
                          'insert (user2,person2@example.com)',
                          '.exit'
                        ])

    result2 = run_script([
                           'select',
                           '.exit'
                         ])

    expect(result).to match_array([
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > Executed.',
                                    'db > '
                                  ])

    expect(result2).to match_array([
                                     'db > (1, user1, person1@example.com)',
                                     '(2, user2, person2@example.com)',
                                     'db > '
                                   ])
  end
end
