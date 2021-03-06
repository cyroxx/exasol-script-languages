--
CREATE PYTHON SET SCRIPT VAREMIT_SIMPLE_SET (a double) EMITS (...) AS
def run(ctx):
    ctx.emit(1)
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_SIMPLE_SET (a double) EMITS (...) AS
def run(ctx):
    ctx.emit(1)
def default_output_columns():
    return "x double"
/
--
CREATE PYTHON SCALAR SCRIPT VAREMIT_SIMPLE_SCALAR (a double) EMITS (...) AS
def run(ctx):
    ctx.emit(1)
/
CREATE PYTHON SCALAR SCRIPT DEFAULT_VAREMIT_SIMPLE_SCALAR (a double) EMITS (...) AS
def run(ctx):
    ctx.emit(1)
def default_output_columns():
    return "x double"
/
--
CREATE PYTHON SCALAR SCRIPT VAREMIT_SIMPLE_ALL_DYN (...) EMITS (...) AS
def run(ctx):
    ctx.emit(1)
/
CREATE PYTHON SCALAR SCRIPT DEFAULT_VAREMIT_SIMPLE_ALL_DYN (...) EMITS (...) AS
def run(ctx):
    ctx.emit(1)
def default_output_columns():
    return "x double"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_SIMPLE_SYNTAX_VAR (...) EMITS ( ...   ) AS
def run(ctx):
    ctx.emit(1)
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_SIMPLE_SYNTAX_VAR (...) EMITS ( ...   ) AS
def run(ctx):
    ctx.emit(1)
def default_output_columns():
    return "x double"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_GENERIC_EMIT (a varchar(100)) EMITS (...) AS
def run(ctx):
    ctx.emit(*([ctx[0]]*exa.meta.output_column_count))
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_GENERIC_EMIT (a varchar(100)) EMITS (...) AS
def run(ctx):
    ctx.emit(*([ctx[0]]*exa.meta.output_column_count))
def default_output_columns():
    return "a varchar(100)"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_ALL_GENERIC (...) EMITS (...) AS
def run(ctx):
    ctx.emit(*([ctx[0]]*exa.meta.output_column_count))
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_ALL_GENERIC (...) EMITS (...) AS
def run(ctx):
    ctx.emit(*([ctx[0]]*exa.meta.output_column_count))
def default_output_columns():
    return "a varchar(100)"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_METADATA_SET_EMIT (...) EMITS(...) AS
def run(ctx):
    ctx.emit(repr(exa.meta.output_column_count), 1)
    for i in range (0,exa.meta.output_column_count):
        ctx.emit(exa.meta.output_columns[i].name, 1)
        ctx.emit(repr(exa.meta.output_columns[i].type), 1)
        ctx.emit(exa.meta.output_columns[i].sql_type, 1)
        ctx.emit(repr(exa.meta.output_columns[i].precision), 1)
        ctx.emit(repr(exa.meta.output_columns[i].scale), 1)
        ctx.emit(repr(exa.meta.output_columns[i].length), 1)
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_METADATA_SET_EMIT (...) EMITS(...) AS
def run(ctx):
    ctx.emit(repr(exa.meta.output_column_count), 1)
    for i in range (0,exa.meta.output_column_count):
        ctx.emit(exa.meta.output_columns[i].name, 1)
        ctx.emit(repr(exa.meta.output_columns[i].type), 1)
        ctx.emit(exa.meta.output_columns[i].sql_type, 1)
        ctx.emit(repr(exa.meta.output_columns[i].precision), 1)
        ctx.emit(repr(exa.meta.output_columns[i].scale), 1)
        ctx.emit(repr(exa.meta.output_columns[i].length), 1)
def default_output_columns():
    return "a varchar(123), b double"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_NON_VAR_EMIT (...) EMITS (a double) AS
def run(ctx):
    ctx.emit(1)
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_NON_VAR_EMIT (...) EMITS (a double) AS
def run(ctx):
    ctx.emit(1)
def default_output_columns():
    return "a int"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_SIMPLE_RETURNS (a int) RETURNS int AS
def run(ctx):
    return 1
def default_output_columns():
    return "x double"
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_SIMPLE_RETURNS (a int) RETURNS int AS
def run(ctx):
    return 1
def default_output_columns():
    return "a int"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_EMIT_INPUT (...) EMITS (...) AS
def run(ctx):
    record = list()
    for col in range(0,exa.meta.input_column_count):
        record.append(ctx[col])
    ctx.emit(*record)
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_EMIT_INPUT (...) EMITS (...) AS
def run(ctx):
    record = list()
    for col in range(0,exa.meta.input_column_count):
        record.append(ctx[col])
    ctx.emit(*record)
def default_output_columns():
    return "a int"
/
--
CREATE PYTHON SET SCRIPT VAREMIT_EMIT_INPUT_WITH_META_CHECK (...) EMITS (...) AS
def run(ctx):
    record = list()
    for col in range(0,exa.meta.input_column_count):
        assert exa.meta.input_columns[col].sql_type == exa.meta.output_columns[col].sql_type
        record.append(ctx[col])
    ctx.emit(*record)
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_EMIT_INPUT_WITH_META_CHECK (...) EMITS (...) AS
def run(ctx):
    record = list()
    for col in range(0,exa.meta.input_column_count):
        assert exa.meta.input_columns[col].sql_type == exa.meta.output_columns[col].sql_type
        record.append(ctx[col])
    ctx.emit(*record)
def default_output_columns():
    return "a varchar(123), b double"
/
CREATE PYTHON SET SCRIPT DEFAULT_VAREMIT_EMPTY_DEF(X DOUBLE) EMITS (...) AS
def run(ctx):
    ctx.emit(1.4)

def default_output_columns():
   return ''
/

CREATE PYTHON SET SCRIPT COPY_RELATION (...) EMITS (...) AS
def run(ctx):
    record = list()
    for col in range(0,exa.meta.input_column_count):
        assert exa.meta.input_columns[col].sql_type == exa.meta.output_columns[col].sql_type
        record.append(ctx[col])
    ctx.emit(*record)

def default_output_columns():
    res = list()
    for col in range(0,exa.meta.input_column_count):
        col_name = exa.meta.input_columns[col].name
        try:
            col_name = "col_%s" % (int(col_name))
        except ValueError: pass
        res.append("%s %s" % (col_name, exa.meta.input_columns[col].sql_type))
    return ",".join(res)
/


