/* -*- mode: c++; -*- */
$ifndef SWIGCONTAINERS_H
$define SWIGCONTAINERS_H



#ifndef EXTERNAL_PROCESS
$include <containers/ContainerTypes.h>
$include <engine/exscript/vmcontainer.h>
$include <engine/exscript/vmchelpers.h>
$include <datatypemgmr/NumericCastBase.h>
$include <datatypemgmr/DirectCastAccess.h>
$include "zmqcontainer.pb.h"
$include <engine/exscript/script_data_transfer_objects_wrapper.h>
#else
$include <vector>
$include <limits>
$include <iostream>
$include <sstream>
$include <zmq.hpp>
$include "zmqcontainer.pb.h"
$include "scriptDTOWrapper.h"
#endif

$include <string>

$include <unistd.h>

namespace SWIGVMContainers {




#ifdef EXTERNAL_PROCESS
void socket_send(zmq::socket_t &socket, zmq::message_t &zmsg);
bool socket_recv(zmq::socket_t &socket, zmq::message_t &zmsg, bool return_on_error = false);
#else
class ZMQConnectionHandler;
ZMQConnectionHandler *zmq_create_internal_handler();
void zmq_delete_internal_handler(ZMQConnectionHandler *&handler);
ZMQConnectionHandler *zmq_create_external_handler(const std::string &redirector_url);
void zmq_delete_external_handler(ZMQConnectionHandler *&handler);
#endif

$define SWIG_MAX_VAR_DATASIZE 4000000

class SWIGVM;
enum VMTYPE {
    VM_UNSUPPORTED = 0,
    VM_PYTHON = 1,
    VM_SCHEME = 2,
    VM_JAVASCRIPT = 3,
    VM_R = 4,
    VM_EXTERNAL = 5,
    VM_JAVA = 6,
    VM_PLUGIN_LANGUAGE = 7
};

struct SWIGVMExceptionHandler
#ifndef EXTERNAL_PROCESS
: public ExecutionGraph::VMCExceptionHandler
#endif
{
    SWIGVMExceptionHandler() { exthrowed = false; }
    ~SWIGVMExceptionHandler() { }
    void setException(const char* msg) {
        exmsg = msg; exthrowed = true;
    }
    std::string exmsg;
    bool exthrowed;
};

enum SWIGVM_datatype_e {
    UNSUPPORTED = 0,
    DOUBLE = 1,
    INT32 = 2,
    INT64 = 3,
    NUMERIC = 4,
    TIMESTAMP = 5,
    DATE = 6,
    STRING = 7,
    BOOLEAN = 8,
    INTERVALYM = 9,
    INTERVALDS = 10,
    GEOMETRY = 11
};

enum SWIGVM_itertype_e {
    EXACTLY_ONCE = 1,
    MULTIPLE = 2
};


struct SWIGVM_columntype_t {
    SWIGVM_datatype_e type;
    std::string type_name;
    unsigned int len;
    unsigned int prec;
    unsigned int scale;
    SWIGVM_columntype_t(const SWIGVM_datatype_e t, const char *n, const unsigned int l, const unsigned int p, const unsigned int s):
        type(t), type_name(n), len(l), prec(p), scale(s) { }
    SWIGVM_columntype_t(const SWIGVM_datatype_e t, const char *n, const unsigned int l): type(t), type_name(n), len(l), prec(0), scale(0) { }
    SWIGVM_columntype_t(const SWIGVM_datatype_e t, const char *n): type(t), type_name(n), len(0), prec(0), scale(0) { }
    SWIGVM_columntype_t(): type(UNSUPPORTED), type_name("UNSUPPORTED"), len(0), prec(0), scale(0) { }
};


struct SWIGVM_params_t {
#ifndef EXTERNAL_PROCESS
    ExecutionGraph::EXScriptVMContainerBasis *vm;
    ExecutionGraph::VMCCachedTableIterator *inp_tbl;
    ExecutionGraph::VMCCachedResultHandler *out_tbl;
    StackAllocator *out_stack;
    ZMQConnectionHandler *zmqhandler;
    SWIGVM *vm_container;
    Mutex mutex;
    bool in_use;
#else
    uint64_t connection_id;
    zmq::socket_t *sock;
#endif
    SWIGVMExceptionHandler *exch;
    char *dbname;
    char *dbversion;
    char *script_name;
    char *script_schema;
    char *current_user;
    char *current_schema;
    char *script_code;
    unsigned long long session_id;
    unsigned long statement_id;
    unsigned int node_count;
    unsigned int node_id;
    unsigned long long vm_id;
    VMTYPE vm_type;
    unsigned long long maximal_memory_limit;

    std::vector<std::string> *inp_names;
    std::vector<SWIGVM_columntype_t> *inp_types;
    SWIGVM_itertype_e inp_iter_type;
    bool inp_force_finish;

    std::vector<std::string> *out_names;
    std::vector<SWIGVM_columntype_t> *out_types;
    SWIGVM_itertype_e out_iter_type;

    bool m_allocate_params;
    std::vector<bool> *is_emitted;
    bool singleCallMode;

    bool isInternalLanguage;
    std::string pluginName;
    std::string pluginURI;
    std::string outputAddress;


    SWIGVM_params_t():
#ifndef EXTERNAL_PROCESS
        vm(NULL), inp_tbl(NULL), out_tbl(NULL), out_stack(NULL), zmqhandler(NULL),
        vm_container(NULL),  mutex("SWIGVM") , in_use(false),
#else
        connection_id(0), sock(NULL),
#endif
        exch(NULL), dbname(NULL), dbversion(NULL), script_name(NULL), script_schema(NULL), current_user(NULL), current_schema(NULL), script_code(NULL),
        session_id(0), statement_id(0), node_count(0), node_id(0), vm_id(0),
        vm_type(VM_UNSUPPORTED), maximal_memory_limit(0),
        inp_names(NULL), inp_types(NULL), inp_iter_type(MULTIPLE), inp_force_finish(false),
        out_names(NULL), out_types(NULL), out_iter_type(MULTIPLE),
        m_allocate_params(false),
        is_emitted(NULL), singleCallMode(false), isInternalLanguage(true), pluginName(""), pluginURI(""), outputAddress("")
    { }

    SWIGVM_params_t(const bool allocate_params):
#ifndef EXTERNAL_PROCESS
        vm(NULL), inp_tbl(NULL), out_tbl(NULL), out_stack(NULL), zmqhandler(NULL),
        vm_container(NULL), mutex("SWIGVM") , in_use(false),
#else
        connection_id(0), sock(NULL),
#endif
        exch(NULL), dbname(NULL), dbversion(NULL), script_name(NULL), script_schema(NULL), current_user(NULL), current_schema(NULL), script_code(NULL),
        session_id(0), statement_id(0), node_count(0), node_id(0), vm_id(0),
        vm_type(VM_UNSUPPORTED), maximal_memory_limit(0),
        inp_names(allocate_params ? new std::vector<std::string>() : NULL),
        inp_types(allocate_params ? new std::vector<SWIGVM_columntype_t>() : NULL),
        inp_iter_type(MULTIPLE),
        inp_force_finish(false),
        out_names(allocate_params ? new std::vector<std::string>() : NULL),
        out_types(allocate_params ? new std::vector<SWIGVM_columntype_t>() : NULL),
        out_iter_type(MULTIPLE),
        m_allocate_params(allocate_params),
        is_emitted(allocate_params ? new std::vector<bool>() : NULL),
        singleCallMode(false), isInternalLanguage(true), pluginName(""), pluginURI(""), outputAddress("")
    { }

    SWIGVM_params_t(const SWIGVM_params_t &p):
#ifndef EXTERNAL_PROCESS
        vm(p.vm),
        inp_tbl(p.inp_tbl),
        out_tbl(p.out_tbl),
        mutex("SWIGVM"),
#else
        connection_id(p.connection_id),
        sock(p.sock),
#endif
        exch(p.exch),
        dbname(p.dbname),
        dbversion(p.dbversion),
        script_name(p.script_name),
        script_schema(p.script_schema),
        current_user(p.current_user),
        current_schema(p.current_schema),
        script_code(p.script_code),
        session_id(p.session_id),
        statement_id(p.statement_id),
        node_count(p.node_count),
        node_id(p.node_id),
        vm_id(p.vm_id),
        vm_type(p.vm_type),
        maximal_memory_limit(p.maximal_memory_limit),
        inp_names(p.inp_names),
        inp_types(p.inp_types),
        inp_iter_type(p.inp_iter_type),
        inp_force_finish(p.inp_force_finish),
        out_names(p.out_names),
        out_types(p.out_types),
        out_iter_type(p.out_iter_type),
        m_allocate_params(false),
        is_emitted(p.is_emitted),
        isInternalLanguage(p.isInternalLanguage),
        pluginName(p.pluginName),
        pluginURI(p.pluginURI),
        outputAddress(p.outputAddress)
    {
        if (p.m_allocate_params) abort();
    }
    ~SWIGVM_params_t() {
        if (m_allocate_params) {
            if (inp_names != NULL) { delete inp_names; inp_names = NULL; }
            if (inp_types != NULL) { delete inp_types; inp_types = NULL; }
            if (out_names != NULL) { delete out_names; out_names = NULL; }
            if (out_types != NULL) { delete out_types; out_types = NULL; }
            if (is_emitted != NULL) { delete is_emitted; is_emitted = NULL; }
        }
    }
};
#ifdef EXTERNAL_PROCESS
extern __thread SWIGVM_params_t *SWIGVM_params;
#endif

class SWIGMetadata {
    public:
        SWIGMetadata(
#ifndef EXTERNAL_PROCESS
                SWIGVM_params_t *SWIGVM_params
#endif
           ):
#ifndef EXTERNAL_PROCESS
            m_vm(SWIGVM_params->vm),
#else
            m_connection_id(SWIGVM_params->connection_id),
            m_socket(*(SWIGVM_params->sock)),
#endif
            m_exch(SWIGVM_params->exch),
            m_db_name(SWIGVM_params->dbname),
            m_db_version(SWIGVM_params->dbversion),
            m_script_name(SWIGVM_params->script_name),
            m_script_schema(SWIGVM_params->script_schema),
            m_current_user(SWIGVM_params->current_user),
            m_current_schema(SWIGVM_params->current_schema),
            m_script_code(SWIGVM_params->script_code),
            m_session_id(SWIGVM_params->session_id),
            m_statement_id(SWIGVM_params->statement_id),
            m_node_count(SWIGVM_params->node_count),
            m_node_id(SWIGVM_params->node_id),
            m_vm_id(SWIGVM_params->vm_id),
            m_input_names(*(SWIGVM_params->inp_names)),
            m_input_types(*(SWIGVM_params->inp_types)),
            m_input_iter_type(SWIGVM_params->inp_iter_type),
            m_output_names(*(SWIGVM_params->out_names)),
            m_output_types(*(SWIGVM_params->out_types)),
            m_output_iter_type(SWIGVM_params->out_iter_type),
            m_memory_limit(SWIGVM_params->maximal_memory_limit),
            m_vm_type(SWIGVM_params->vm_type),
            m_is_emitted(*(SWIGVM_params->is_emitted)),
            m_isInternalLanguage(SWIGVM_params->isInternalLanguage),
            m_pluginLanguageName(SWIGVM_params->pluginName),
            m_pluginURI(SWIGVM_params->pluginURI),
            m_outputAddress(SWIGVM_params->outputAddress)
        {
            { std::stringstream sb; sb << m_session_id; m_session_id_s = sb.str(); }
            { std::stringstream sb; sb << m_vm_id; m_vm_id_s = sb.str(); }
        }
        virtual ~SWIGMetadata() { }

        inline const char* databaseName() { return m_db_name.c_str(); }
        inline const char* databaseVersion() { return m_db_version.c_str(); }
        inline const char* scriptName() { return m_script_name.c_str(); }
        inline const char* scriptSchema() { return m_script_schema.c_str(); }
        inline const char* currentUser() { return m_current_user.c_str(); }
        inline const char* currentSchema() {return m_current_schema.c_str();}
        inline const char* scriptCode() { return m_script_code.c_str(); }
        inline const unsigned long long sessionID() { return m_session_id; }
        inline const char *sessionID_S() { return m_session_id_s.c_str(); }
        inline const unsigned long statementID() { return m_statement_id; }
        inline const unsigned int nodeCount() { return m_node_count; }
        inline const unsigned int nodeID() { return m_node_id; }
        inline const unsigned long long vmID() { return m_vm_id; }
        inline const unsigned long long memoryLimit() { return m_memory_limit; }
        inline const VMTYPE vmType() { return m_vm_type; }
        inline const char *vmID_S() { return m_vm_id_s.c_str(); }
        inline const ExecutionGraph::ConnectionInformationWrapper connectionInformation(const char* connection_name)
        {
#ifndef EXTERNAL_PROCESS
            try {
                ExecutionGraph::ConnectionInformation ci = m_vm->getConnectionInfo(connection_name);
                return ExecutionGraph::ConnectionInformationWrapper(ci);
            } catch (SyntaxErrorOrAccessRuleViolation &err) {
                m_exch->setException(err.what());
            }

            return ExecutionGraph::ConnectionInformationWrapper(ExecutionGraph::ConnectionInformation());
#else
            exascript_request request;
            request.set_type(MT_IMPORT);
            request.set_connection_id(m_connection_id);
            exascript_import_req *req = request.mutable_import();
            req->set_script_name(connection_name);
            req->set_kind(PB_IMPORT_CONNECTION_INFORMATION);
            if (!request.SerializeToString(&m_output_buffer)) {
                m_exch->setException("Communication error: failed to serialize data");
                return ExecutionGraph::ConnectionInformationWrapper(ExecutionGraph::ConnectionInformation());
            }
            zmq::message_t zmsg_req((void*)m_output_buffer.c_str(), m_output_buffer.length(), NULL, NULL);
            socket_send(m_socket, zmsg_req);
            zmq::message_t zmsg_rep;
            socket_recv(m_socket, zmsg_rep);
            exascript_response response;
            if (!response.ParseFromArray(zmsg_rep.data(), zmsg_rep.size())) {
                m_exch->setException("Communication error: failed to parse data");
                return ExecutionGraph::ConnectionInformationWrapper(ExecutionGraph::ConnectionInformation());
            }
            if (response.type() != MT_IMPORT) {
                m_exch->setException("Internal error: wrong message type");
                return ExecutionGraph::ConnectionInformationWrapper(ExecutionGraph::ConnectionInformation());
            }
            const exascript_import_rep &rep = response.import();
            if (rep.has_exception_message()) {
                m_exch->setException(rep.exception_message().c_str());
                return ExecutionGraph::ConnectionInformationWrapper(ExecutionGraph::ConnectionInformation());
            }
            if (!rep.has_connection_information()) {
                m_exch->setException("Internal error: No connection information returned");
                return ExecutionGraph::ConnectionInformationWrapper(ExecutionGraph::ConnectionInformation());
            }
            connection_information_rep ci = rep.connection_information();
            return ExecutionGraph::ConnectionInformationWrapper(
                        ExecutionGraph::ConnectionInformation(ci.kind(), ci.address(), ci.user(), ci.password()));
#endif
        }

        inline const char* moduleContent(const char* name) {
#ifndef EXTERNAL_PROCESS
            try {
                RefSchemaScript s = m_vm->getIncludeScript(name);
                m_temp_code = s->getText().c_str();
                return m_temp_code.c_str();
            } catch (SyntaxErrorOrAccessRuleViolation &err) {
                m_exch->setException(err.what());
            }
            return NULL;
#else
            exascript_request request;
            request.set_type(MT_IMPORT);
            request.set_connection_id(m_connection_id);
            exascript_import_req *req = request.mutable_import();
            req->set_script_name(name);
            req->set_kind(PB_IMPORT_SCRIPT_CODE);
            if (!request.SerializeToString(&m_output_buffer)) {
                m_exch->setException("Communication error: failed to serialize data");
                return NULL;
            }
            zmq::message_t zmsg_req((void*)m_output_buffer.c_str(), m_output_buffer.length(), NULL, NULL);
            socket_send(m_socket, zmsg_req);
            zmq::message_t zmsg_rep;
            socket_recv(m_socket, zmsg_rep);
            exascript_response response;
            if (!response.ParseFromArray(zmsg_rep.data(), zmsg_rep.size())) {
                m_exch->setException("Communication error: failed to parse data");
                return NULL;
            }
            if (response.type() != MT_IMPORT) {
                m_exch->setException("Internal error: wrong message type");
                return NULL;
            }
            const exascript_import_rep &rep = response.import();
            if (rep.has_exception_message()) {
                m_exch->setException(rep.exception_message().c_str());
                return NULL;
            }
            if (!rep.has_source_code()) {
                m_exch->setException("Internal error: No source code returned");
                return NULL;
            }
            m_temp_code = rep.source_code();
            return m_temp_code.c_str();
#endif
        }

        inline const unsigned int inputColumnCount() { return m_input_names.size(); }
        inline const char *inputColumnName(unsigned int col)
        { return col >= m_input_names.size() ? NULL : m_input_names[col].c_str(); }
        inline const SWIGVM_datatype_e inputColumnType(unsigned int col)
        { return col >= m_input_types.size() ? UNSUPPORTED : m_input_types[col].type; }
        inline const char *inputColumnTypeName(unsigned int col)
        { return col >= m_input_types.size() ? NULL : m_input_types[col].type_name.c_str(); }
        inline const unsigned int inputColumnSize(unsigned int col)
        { return col >= m_input_types.size() ? 0 : m_input_types[col].len; }
        inline const unsigned int inputColumnPrecision(unsigned int col)
        { return col >= m_input_types.size() ? 0 : m_input_types[col].prec; }
        inline const unsigned int inputColumnScale(unsigned int col)
        { return col >= m_input_types.size() ? 0 : m_input_types[col].scale; }
        inline const SWIGVM_itertype_e inputType() { return m_input_iter_type; }

        inline const unsigned int outputColumnCount() { return m_output_names.size(); }
        inline const char *outputColumnName(unsigned int col) {
            if (m_output_iter_type == EXACTLY_ONCE && col == 0)
                return "RETURN";
            return col >= m_output_names.size() ? NULL : m_output_names[col].c_str();
        }
        inline const SWIGVM_datatype_e outputColumnType(unsigned int col)
        { return col >= m_output_types.size() ? UNSUPPORTED : m_output_types[col].type; }
        inline const char *outputColumnTypeName(unsigned int col)
        { return col >= m_output_types.size() ? NULL : m_output_types[col].type_name.c_str(); }
        inline const unsigned int outputColumnSize(unsigned int col)
        { return col >= m_output_types.size() ? 0 : m_output_types[col].len; }
        inline const unsigned int outputColumnPrecision(unsigned int col)
        { return col >= m_output_types.size() ? 0 : m_output_types[col].prec; }
        inline const unsigned int outputColumnScale(unsigned int col)
        { return col >= m_output_types.size() ? 0 : m_output_types[col].scale; }
        inline const SWIGVM_itertype_e outputType() { return m_output_iter_type; }

        inline const bool isEmittedColumn(unsigned int col){
            if (col >= m_is_emitted.size())
            {
                abort();
            }
            return m_is_emitted[col];
        }

        inline const char* checkException() {
            if (m_exch->exthrowed) {
                m_exch->exthrowed = false;
                return m_exch->exmsg.c_str();
            } else return NULL;
        }


        inline const bool isInternalLanguage()
        {
            return m_isInternalLanguage;
        }

        inline const char* pluginLanguageName()
        {
            return m_pluginLanguageName.c_str();
        }

        inline const char* pluginURI()
        {
            return m_pluginURI.c_str();
        }

        inline const char* outputAddress()
        {
            return m_outputAddress.c_str();
        }

    private:
#ifndef EXTERNAL_PROCESS
        ExecutionGraph::EXScriptVMContainerBasis *m_vm;
#else
        const uint64_t m_connection_id;
        zmq::socket_t &m_socket;
        std::string m_output_buffer;
#endif
        SWIGVMExceptionHandler *m_exch;
        const std::string m_db_name;
        const std::string m_db_version;
        const std::string m_script_name;
        const std::string m_script_schema;
        const std::string m_current_user;
        const std::string m_current_schema;
        const std::string m_script_code;
        const unsigned long m_session_id;
        const unsigned long m_statement_id;
        const unsigned int m_node_count;
        const unsigned int m_node_id;
        const unsigned long m_vm_id;
        std::string m_temp_code;

        const std::vector<std::string> &m_input_names;
        const std::vector<SWIGVM_columntype_t> &m_input_types;
        const SWIGVM_itertype_e m_input_iter_type;

        const std::vector<std::string> &m_output_names;
        const std::vector<SWIGVM_columntype_t> &m_output_types;
        const SWIGVM_itertype_e m_output_iter_type;

        const unsigned long long m_memory_limit;
        const std::string m_meta_info;
        const VMTYPE m_vm_type;
        std::string m_session_id_s;
        std::string m_vm_id_s;
        const std::vector<bool> &m_is_emitted;

        const bool m_isInternalLanguage;
        std::string m_pluginLanguageName;
        std::string m_pluginURI;
        std::string m_outputAddress;
};

class SWIGGeneralIterator {
    protected:
        SWIGVMExceptionHandler *m_exch;

    public:
        SWIGGeneralIterator(SWIGVMExceptionHandler *exch): m_exch(exch) { }
        virtual ~SWIGGeneralIterator() { }
        inline const char* checkException() {
            if (m_exch->exthrowed) {
                m_exch->exthrowed = false;
                return m_exch->exmsg.c_str();
            } else return NULL;
        }
};

class SWIGTableIterator: public SWIGGeneralIterator {
    private:
#ifndef EXTERNAL_PROCESS
        SWIGVM_params_t *SWIGVM_params;
        ExecutionGraph::VMCCachedTableIterator &m_table;
        size_t m_string_buffer_length;
        char *m_string_buffer_data;
#else
        const uint64_t m_connection_id;
        zmq::socket_t &m_socket;
        std::string m_output_buffer;
        exascript_request m_request;
        exascript_response m_next_response;
        uint64_t m_rows_received;
        struct values_per_row_t {
            uint64_t strings, bools, int32s, int64s, doubles;
            values_per_row_t(): strings(0), bools(0), int32s(0), int64s(0), doubles(0) {}
            void reset() { strings = bools = int32s = int64s = doubles = 0; }
        } m_values_per_row;
        uint64_t m_column_count;
        std::vector<uint64_t> m_col_offsets;
        uint64_t m_rows_in_group;
        uint64_t m_current_row;
#endif
        uint64_t m_rows_completed;
        uint64_t m_rows_group_completed;
        bool m_was_null;
        const std::vector<SWIGVM_columntype_t> &m_types;

#ifdef EXTERNAL_PROCESS
        void increment_col_offsets(bool reset = false) {
            m_current_row = m_next_response.next().table().row_number(m_rows_completed); //local row number
            uint64_t current_column = 0;
            ssize_t null_index = 0;
            if (reset) m_values_per_row.reset();
            null_index = m_rows_completed * m_column_count;
            if (m_next_response.next().table().data_nulls_size() <= (null_index + (ssize_t)m_column_count - 1)) {
                std::stringstream sb;
                sb << "Internal error: not enough nulls in packet: wanted index " << (null_index + m_column_count - 1)
                   << " but have " << m_next_response.next().table().data_nulls_size()
                   << " elements";
                m_exch->setException(sb.str().c_str());
                return;
            }
            for (std::vector<SWIGVM_columntype_t>::const_iterator
                     it = m_types.begin(); it != m_types.end(); ++it, ++current_column, ++null_index)
            {
                if (m_next_response.next().table().data_nulls(null_index))
                    continue;
                switch (it->type) {
                case UNSUPPORTED: m_exch->setException("Unsupported data type found"); return;
                case DOUBLE: m_col_offsets[current_column] = m_values_per_row.doubles++; break;
                case INT32: m_col_offsets[current_column] = m_values_per_row.int32s++; break;
                case INT64: m_col_offsets[current_column] = m_values_per_row.int64s++; break;
                case NUMERIC:
                case TIMESTAMP:
                case DATE:
                case STRING: m_col_offsets[current_column] = m_values_per_row.strings++; break;
                case BOOLEAN: m_col_offsets[current_column] = m_values_per_row.bools++; break;
                default: m_exch->setException("Unknown data type found"); return;
                }
            }
        }

        void receive_next_data(bool reset) {
            m_rows_received = 0;
            m_rows_completed = 0;
            {   // send request
                m_request.Clear();
                m_request.set_connection_id(m_connection_id);
                if (reset) m_request.set_type(MT_RESET);
                else m_request.set_type(MT_NEXT);
                if(!m_request.SerializeToString(&m_output_buffer)) {
                    m_exch->setException("Communication error: failed to serialize data");
                    return;
                }
                zmq::message_t zmsg((void*)m_output_buffer.c_str(), m_output_buffer.length(), NULL, NULL);
                socket_send(m_socket, zmsg);
            } { // receive response
                zmq::message_t zmsg;
                socket_recv(m_socket, zmsg);
                m_next_response.Clear();
                if (!m_next_response.ParseFromArray(zmsg.data(), zmsg.size())) {
                    m_exch->setException("Communication error: failed to parse data");
                    return;
                }
                if (m_next_response.connection_id() != m_connection_id) {
                    m_exch->setException("Communication error: wrong connection id");
                    return;
                }
                if (m_next_response.type() == MT_DONE) {
                    return;
                }
                if (m_next_response.type() == MT_CLOSE) {
                    const exascript_close &rep = m_next_response.close();
                    if (!rep.has_exception_message()) {
                        if (m_rows_completed == 0) {
                            return;
                        } else m_exch->setException("Unknown error occured");
                    } else {
                        m_exch->setException(rep.exception_message().c_str());
                    }
                    return;
                }
                if ((reset && (m_next_response.type() != MT_RESET)) ||
                   (!reset && (m_next_response.type() != MT_NEXT)))
                {
                    m_exch->setException("Communication error: wrong message type");
                    return;
                }
                m_rows_received = m_next_response.next().table().rows();
                m_rows_in_group = m_next_response.next().table().rows_in_group();
            }
            increment_col_offsets(true);
        }

        inline ssize_t check_index(ssize_t index, ssize_t available, const char *tp, const char *otype, const char *ts) {
            if (available > index) return index;
            std::stringstream sb;
            sb << "Internal error: not enough " << tp << otype << ts << " in packet: wanted index "
               << index << " but have " << available << " elements (on " 
               << m_rows_received << '/' << m_rows_completed << " of received/completed rows";
            m_exch->setException(sb.str().c_str());
            m_was_null = true;
            return -1;
        }

        inline ssize_t check_value(unsigned int col, ssize_t available, const char *otype) {
            m_was_null = false;
            ssize_t index = check_index(m_column_count * m_rows_completed + col,
                                        m_next_response.next().table().data_nulls_size(),
                                        "nulls for ", otype, "");
            if (m_was_null) return -1;
            m_was_null = m_next_response.next().table().data_nulls(index);
            if (m_was_null) return -1;
            index = check_index(m_col_offsets[col], available, "", otype, "s");
            if (m_was_null) return -1;
            return index;
        }
#else
        inline char *string_buffer(size_t length) {
            if (length >= m_string_buffer_length) {
                m_string_buffer_length = length;
                delete[] m_string_buffer_data;
                m_string_buffer_data = new char[m_string_buffer_length + 1];
            }
            return m_string_buffer_data;
        }

        inline char *string_buffer(const char* str, size_t length) {
            char *buf = string_buffer(length);
            memcpy(buf, str, length);
            buf[length] = '\0';
            return buf;
        }

        inline char *string_buffer(const MString &str) {
            size_t strlength = str.length();
            char *buf = string_buffer(strlength);
            memcpy(buf, str.c_str(), strlength);
            buf[strlength] = '\0';
            return buf;
        }

        inline char *string_buffer(const std::string &str) {
            size_t strlength = str.length();
            char *buf = string_buffer(strlength);
            memcpy(buf, str.c_str(), strlength);
            buf[strlength] = '\0';
            return buf;
        }
#endif
    
    public:
        SWIGTableIterator(
#ifndef EXTERNAL_PROCESS
                SWIGVM_params_t *params
#endif
            ):
#ifndef EXTERNAL_PROCESS
            SWIGGeneralIterator(params->exch),
            SWIGVM_params(params),
            m_table(*(SWIGVM_params->inp_tbl)),
            m_string_buffer_length(1024),
            m_string_buffer_data(new char[m_string_buffer_length + 1]),
#else
            SWIGGeneralIterator(SWIGVM_params->exch),
            m_connection_id(SWIGVM_params->connection_id),
            m_socket(*(SWIGVM_params->sock)),
            m_column_count(SWIGVM_params->inp_types->size()),
            m_col_offsets(SWIGVM_params->inp_types->size()),
            m_current_row((uint64_t)-1),
#endif
            m_rows_completed(0),
            m_rows_group_completed(1),                                                         
            m_was_null(false),
            m_types(*(SWIGVM_params->inp_types))
        {
#ifdef EXTERNAL_PROCESS
            receive_next_data(false);
#endif
        }

        ~SWIGTableIterator() {
#ifndef EXTERNAL_PROCESS
            delete[] m_string_buffer_data;
#else
            //std::cerr << "#### SWIGVM " << m_connection_id << " TableIterator destructor" << std::endl;
#endif
        }

#ifdef EXTERNAL_PROCESS
        //returns the local row number of the row last read (used by emit to set local row number for emitted rows)
        uint64_t get_current_row()
        {
            return m_current_row;
        }
#endif

        inline void reinitialize() {
#ifdef EXTERNAL_PROCESS
            m_rows_completed = 0;
            m_rows_group_completed = 1;
            m_values_per_row.reset();
            receive_next_data(false);
#endif
        }

        inline bool next() {
#ifndef EXTERNAL_PROCESS
            if (m_table.next()) {
                ++m_rows_completed;
                ++m_rows_group_completed;
                return true;
            }
            return false;
#else
            if (m_rows_received == 0) {
                m_exch->setException("Iteration finished");
                return false;
            }
            ++m_rows_completed;
            ++m_rows_group_completed;
            if (SWIGVM_params->inp_force_finish)
                return false;
            if (m_rows_completed >= m_rows_received) {
                receive_next_data(false);
                if (m_rows_received == 0)
                    return false;
                else return true;
            }
            increment_col_offsets();
            return true;
#endif
        }

        inline bool eot() {
#ifndef EXTERNAL_PROCESS
            return false; // TODO: dont workind now
#else
            return m_rows_received == 0;
#endif
        }

        inline void reset() {
            m_rows_group_completed = 1;
#ifndef EXTERNAL_PROCESS
            m_table.reset();
            m_rows_completed = 0;
#else
            SWIGVM_params->inp_force_finish = false;
            receive_next_data(true);
#endif
        }

        inline unsigned long restBufferSize() {
#ifdef EXTERNAL_PROCESS
            if (m_rows_completed < m_rows_received)
                return m_rows_received - m_rows_completed;
#endif
            return 0;
        }

        inline unsigned long rowsCompleted() {
            return m_rows_group_completed;
        }
    
        inline unsigned long rowsInGroup() {
#ifdef EXTERNAL_PROCESS
            return m_rows_in_group;
#else
            return m_table.size();
#endif
        }
    
        inline double getDouble(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return 0.0; }
            if (m_types[col].type != DOUBLE) {
                m_exch->setException("Wrong column type");
                m_was_null = true;
                return 0.0;
            }
#ifndef EXTERNAL_PROCESS
            double ret = 0.0;
            m_was_null = !m_table.get<double>(col, ret);
            if (m_was_null) return 0.0;
            return ret;
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_double_size(), "double");
            if (m_was_null) {
                return 0.0;
            }
            return m_next_response.next().table().data_double(index);
#endif
        }

        inline const char *getString(unsigned int col, size_t *length = NULL) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return ""; }
            if (m_types[col].type != STRING) {
                m_exch->setException("Wrong column type");
                m_was_null = true;
                return "";
            }
#ifndef EXTERNAL_PROCESS
            StringData ret;
            ret.string = (char*)"";
            ret.length = 0;
            m_was_null = !m_table.get<StringData>(col, ret);
            if (m_was_null) return "";
            if (length != NULL) {
                *length = ret.length;
                return ret.string;
            }
            return string_buffer(ret.string, ret.length);
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_string_size(), "string");
            if (m_was_null) return "";
            const std::string &s(m_next_response.next().table().data_string(index));
            if (length != NULL) *length = s.length();
            return s.c_str();
#endif
        }

        inline int32_t getInt32(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return 0; }
            if (m_types[col].type != INT32) {
                m_exch->setException("Wrong column type");
                m_was_null = true;
                return 0;
            }
#ifndef EXTERNAL_PROCESS
            int32_t ret = 0;
            m_was_null = !m_table.get<smallint>(col, ret);
            if (m_was_null) return 0;
            return ret;
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_int32_size(), "int32");
            if (m_was_null) return 0;
            return m_next_response.next().table().data_int32(index);
#endif
        }

        inline int64_t getInt64(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return 0LL; }
            if (m_types[col].type != INT64) {
                m_exch->setException("Wrong column type");
                m_was_null = true;
                return 0LL;
            }
#ifndef EXTERNAL_PROCESS
            int64_t ret = 0;
            m_was_null = !m_table.get<integer>(col, ret);
            if (m_was_null) return 0;
            return ret;
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_int64_size(), "int64");
            if (m_was_null) return 0LL;
            return m_next_response.next().table().data_int64(index);
#endif
        }

        inline const char *getNumeric(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return ""; }
            if (m_types[col].type != NUMERIC) { m_exch->setException("Wrong column type"); m_was_null = true; return ""; }
#ifndef EXTERNAL_PROCESS
            if (m_types[col].len == 4) {
                smallint ret = 0;
                m_was_null = !m_table.get<smallint>(col, ret);
                if (m_was_null) return "0";
                m_string_buffer_data[DTM::numeric_to_string<smallint>(m_string_buffer_data, m_string_buffer_length, ret, m_types[col].scale, ".,")] = '\0';
                return m_string_buffer_data;
            }
            if (m_types[col].len == 8) {
                integer ret = 0;
                m_was_null = !m_table.get<integer>(col, ret);
                if (m_was_null) return "0";
                m_string_buffer_data[DTM::numeric_to_string<integer>(m_string_buffer_data, m_string_buffer_length, ret, m_types[col].scale, ".,")] = '\0';
                return m_string_buffer_data;
            }
            if (m_types[col].len == 16) {
                int128_t ret = 0;
                m_was_null = !m_table.get<int128_t>(col, ret);
                if (m_was_null) return "0";
                m_string_buffer_data[DTM::numeric_to_string<int128_t>(m_string_buffer_data, m_string_buffer_length, ret, m_types[col].scale, ".,")] = '\0';
                return m_string_buffer_data;
            }
            return "0";
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_string_size(), "string");
            if (m_was_null) return "0";
            return m_next_response.next().table().data_string(index).c_str();
#endif
        }

        inline const char *getTimestamp(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return ""; }
            if (m_types[col].type != TIMESTAMP) { m_exch->setException("Wrong column type"); m_was_null = true; return ""; }
#ifndef EXTERNAL_PROCESS
            timestamp_t data = 0;
            m_was_null = !m_table.get<timestamp_t>(col, data);
            if (m_was_null) return "1970-01-01 00:00:00.00 0000";
            return string_buffer(DTM::timestamp_to_string(7, data, "YYYY-MM-DD HH24:MI:SS.FF6"));
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_string_size(), "string");
            if (m_was_null) return "1970-01-01 00:00:00.00 0000";
            return m_next_response.next().table().data_string(index).c_str();
#endif
        }

        inline const char *getDate(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return ""; }
            if (m_types[col].type != DATE) { m_exch->setException("Wrong column type"); m_was_null = true; return ""; }
#ifndef EXTERNAL_PROCESS
            date data = min_date;
            m_was_null = !m_table.get<date>(col, data);
            if (m_was_null) return "1970-01-01";
            return string_buffer(DTM::date_to_string(7, data, "YYYY-MM-DD"));
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_string_size(), "string");
            if (m_was_null) return "1970-01-01";
            return m_next_response.next().table().data_string(index).c_str();
#endif
        }

        inline bool getBoolean(unsigned int col) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); m_was_null = true; return ""; }
            if (m_types[col].type != BOOLEAN) { m_exch->setException("Wrong column type"); m_was_null = true; return ""; }
#ifndef EXTERNAL_PROCESS
            BooleanData ret = V_FALSE;
            m_was_null = !m_table.get<BooleanData>(col, ret);
            if (m_was_null) return false;
            return ret == V_TRUE;
#else
            ssize_t index = check_value(col, m_next_response.next().table().data_bool_size(), "bool");
            if (m_was_null) return false;
            return m_next_response.next().table().data_bool(index);
#endif
        }

        inline bool wasNull() { return m_was_null; }
};

class SWIGResultHandler: public SWIGGeneralIterator {
    private:
#ifdef EXTERNAL_PROCESS
        SWIGTableIterator* m_table_iterator;
        const uint64_t m_connection_id;
        zmq::socket_t &m_socket;
        std::string m_output_buffer;
        uint64_t m_message_size;

        struct rowdata_t {
            std::map<int, bool> null_data;
            std::map<int, bool> bool_data;
            std::map<int, double> double_data;
            std::map<int, int32_t> int32_data;
            std::map<int, int64_t> int64_data;
            std::map<int, std::string> string_data;
        };

        rowdata_t m_rowdata;
        exascript_request m_emit_request;
#else
        SWIGVM_params_t *SWIGVM_params;
        StackAllocator &m_stack;
        uint64_t m_stack_size_offset;
        ExecutionGraph::VMCCachedResultHandler &m_table;
        ExecutionGraph::VMCCachedTableIterator &m_input_table;
#endif
        uint64_t m_rows_emited;
        const std::vector<SWIGVM_columntype_t> &m_types;

    public:

        SWIGResultHandler(
#ifndef EXTERNAL_PROCESS
                SWIGVM_params_t *params
#else
                SWIGTableIterator* table_iterator
#endif
            ):
#ifndef EXTERNAL_PROCESS
            SWIGGeneralIterator(params->exch),
            SWIGVM_params(params),
            m_stack(*(SWIGVM_params->out_stack)),
            m_stack_size_offset(m_stack.getAllocatedMemory()),
            m_table(*(SWIGVM_params->out_tbl)),
            m_input_table(*(SWIGVM_params->inp_tbl)),
#else
            SWIGGeneralIterator(SWIGVM_params->exch),
            m_table_iterator(table_iterator),
            m_connection_id(SWIGVM_params->connection_id),
            m_socket(*(SWIGVM_params->sock)),
            m_message_size(0),
#endif
            m_rows_emited(1),
            m_types(*(SWIGVM_params->out_types))
        { }

        ~SWIGResultHandler() {
#ifdef EXTERNAL_PROCESS
            //std::cerr << "#### SWIGVM " << m_connection_id << " ResultHandler destructor" << std::endl;
#endif
        }

        inline void reinitialize() {
            m_rows_emited = 0;
#ifdef EXTERNAL_PROCESS
            m_message_size = 0;
            m_emit_request.Clear();
            m_rowdata = rowdata_t();
#endif
        }

        inline unsigned long rowsEmited() {
            return m_rows_emited;
        }

        inline void flush() {
#ifndef EXTERNAL_PROCESS
            m_table.flush<false>();
#else
            exascript_emit_data_req *req = m_emit_request.mutable_emit();
            exascript_table_data *table = req->mutable_table();
            if (table->has_rows() && table->rows() > 0) {
                {   m_emit_request.set_type(MT_EMIT);
                    m_emit_request.set_connection_id(m_connection_id);
                    if (!m_emit_request.SerializeToString(&m_output_buffer)) {
                        m_exch->setException("Communication error: failed to serialize data");
                        return;
                    }
                    zmq::message_t zmsg((void*)m_output_buffer.c_str(), m_output_buffer.length(), NULL, NULL);
                    socket_send(m_socket, zmsg);
                    m_emit_request.Clear();
                    m_message_size = 0;
                }
                {   zmq::message_t zmsg;
                    socket_recv(m_socket, zmsg);
                    exascript_response response;
                    if (!response.ParseFromArray(zmsg.data(), zmsg.size())) {
                        m_exch->setException("Communication error: failed to parse data");
                        return;
                    }
                    if (response.connection_id() != m_connection_id) {
                        std::stringstream sb;
                        sb << "Received wrong connection id " << response.connection_id()
                           << ", should be " << m_connection_id;
                        m_exch->setException(sb.str().c_str());
                        return;
                    }
                    if (response.type() == MT_CLOSE) {
                        if (!response.close().has_exception_message())
                            m_exch->setException("Unknown error occured");
                        else m_exch->setException(response.close().exception_message().c_str());
                        return;
                    }
                    if (response.type() != MT_EMIT) {
                        m_exch->setException("Wrong response type");
                        return;
                    }
                }
            }
#endif
        }

        inline bool next() {
            ++m_rows_emited;
#ifndef EXTERNAL_PROCESS            
            return m_table.next<false>();
#else
            exascript_emit_data_req *req = m_emit_request.mutable_emit();
            exascript_table_data *table = req->mutable_table();
            for (unsigned int col = 0; col < m_types.size(); ++col) {
                bool null_data = m_rowdata.null_data[col];
                table->add_data_nulls(null_data);
                if (null_data) continue;
                switch (m_types[col].type) {
                    case UNSUPPORTED:
                        m_exch->setException("Unsupported data type found");
                        return false;
                    case DOUBLE:
                        if (m_rowdata.double_data.find(col) == m_rowdata.double_data.end()) {
                            m_exch->setException("Not enough double columns emited");
                            return false;
                        }
                        m_message_size += sizeof(double);
                        table->add_data_double(m_rowdata.double_data[col]);
                        break;
                    case INT32:
                        if (m_rowdata.int32_data.find(col) == m_rowdata.int32_data.end()) {
                            m_exch->setException("Not enough int32 columns emited");
                            return false;
                        }
                        m_message_size += sizeof(int32_t);
                        table->add_data_int32(m_rowdata.int32_data[col]);
                        break;
                    case INT64:
                        if (m_rowdata.int64_data.find(col) == m_rowdata.int64_data.end()) {
                            m_exch->setException("Not enough int64 columns emited");
                            return false;
                        }
                        m_message_size += sizeof(int64_t);
                        table->add_data_int64(m_rowdata.int64_data[col]);
                        break;
                    case NUMERIC:
                    case TIMESTAMP:
                    case DATE:
                    case STRING:
                        if (m_rowdata.string_data.find(col) == m_rowdata.string_data.end()) {
                            m_exch->setException("Not enough string columns emited");
                            return false;
                        }
                        m_message_size += sizeof(int32_t) + m_rowdata.string_data[col].length();
                        *table->add_data_string() = m_rowdata.string_data[col];
                        break;
                    case BOOLEAN:
                        if (m_rowdata.bool_data.find(col) == m_rowdata.bool_data.end()) {
                            m_exch->setException("Not enough boolean columns emited");
                            return false;
                        }
                        m_message_size += 1;
                        table->add_data_bool(m_rowdata.bool_data[col]);
                        break;
                    default:
                        m_exch->setException("Unknown data type found");
                        return false;
                }
            }
            // add local (input) row number for emitted row
            table->add_row_number(m_table_iterator->get_current_row());
            m_rowdata = rowdata_t();
            if (!table->has_rows()) table->set_rows(1);
            else table->set_rows(table->rows() + 1);
            table->set_rows_in_group(0);
            if (m_message_size >= SWIG_MAX_VAR_DATASIZE) {
                if (SWIGVM_params->inp_iter_type == EXACTLY_ONCE && SWIGVM_params->out_iter_type == EXACTLY_ONCE)
                    SWIGVM_params->inp_force_finish = true;
                else this->flush();
            }
            return true;
#endif
        }

        inline void setDouble(unsigned int col, const double v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != DOUBLE) { m_exch->setException("Wrong column type (not a double)"); return; }
#ifndef EXTERNAL_PROCESS
            m_table.set<double>(col, v);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.double_data[col] = v;
#endif
        }

        inline void setString(unsigned int col, const char *v, size_t l) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != STRING) { m_exch->setException("Wrong column type (not a string)"); return; }
#ifndef EXTERNAL_PROCESS
            StringData arg;
            arg.length = l;
            arg.string = alloc_mword_aligned<char>(m_stack, l + 1);
            ::memcpy(arg.string, v, l);
            arg.string[l] = '\0';
            m_table.set(col, arg, m_types[col].len);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.string_data[col] = v;
#endif
        }

        inline void setInt32(unsigned int col, const int32_t v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != INT32) { m_exch->setException("Wrong column type (not Int32)"); return; }
#ifndef EXTERNAL_PROCESS
            m_table.set<smallint>(col, v);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.int32_data[col] = v;
#endif
        }

        inline void setInt64(unsigned int col, const int64_t v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != INT64) { m_exch->setException("Wrong column type (not Int64)"); return; }
#ifndef EXTERNAL_PROCESS
            m_table.set<integer>(col, v);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.int64_data[col] = v;
#endif
        }

        inline void setNumeric(unsigned int col, const char *v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != NUMERIC) { m_exch->setException("Wrong column type (not Numeric)"); return; }
#ifndef EXTERNAL_PROCESS
            if (m_types[col].len == 4) {
                smallint data;
                DTM::string_to_numeric<smallint>(v, strlen(v), data, m_types[col].prec, m_types[col].scale, ".,", true);
                m_table.set<smallint>(col, data);
            } else if (m_types[col].len == 8) {
                integer data;
                DTM::string_to_numeric<integer>(v, strlen(v), data, m_types[col].prec, m_types[col].scale, ".,", true);
                m_table.set<integer>(col, data);
            } else if (m_types[col].len == 16) {
                int128_t data;
                DTM::string_to_numeric<int128_t>(v, strlen(v), data, m_types[col].prec, m_types[col].scale, ".,", true);
                m_table.set<int128_t>(col, data);
            } else m_exch->setException("Numeric column length not supported");
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.string_data[col] = v;
#endif
        }
        inline void setTimestamp(unsigned int col, const char *v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != TIMESTAMP) { m_exch->setException("Wrong column type (not Timestamp)"); return; }
#ifndef EXTERNAL_PROCESS
            timestamp_t data = DTM::string_to_timestamp(7, v, strlen(v), "YYYY-MM-DD HH24:MI:SS.FF6", 25);
            m_table.set<timestamp_t>(col, data);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.string_data[col] = v;
#endif
        }
        inline void setDate(unsigned int col, const char *v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != DATE) { m_exch->setException("Wrong column type (not Date)"); return; }
#ifndef EXTERNAL_PROCESS
            date data = DTM::string_to_date(7, v, strlen(v), "YYYY-MM-DD", 10);
            m_table.set<date>(col, data);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.string_data[col] = v;
#endif
        }
        inline void setBoolean(unsigned int col, const bool v) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != BOOLEAN) { m_exch->setException("Wrong column type (not Boolean)"); return; }
#ifndef EXTERNAL_PROCESS
            BooleanData arg = v ? V_TRUE : V_FALSE;
            m_table.set<BooleanData>(col, arg);
#else
            m_rowdata.null_data[col] = false;
            m_rowdata.bool_data[col] = v;
#endif
        }
        inline void setNull(unsigned int col) {
#ifndef EXTERNAL_PROCESS
            m_table.setNull(col);
#else
            m_rowdata.null_data[col] = true;
#endif
        }

#ifndef EXTERNAL_PROCESS
        // random access on input table (used for expressions in select list)
        inline bool saveInTableState(size_t& numReads, size_t& cpos)
        {
            return m_input_table.saveState(numReads, cpos);
        }
        inline void restoreInTableState(size_t numReads, size_t cpos)
        {
            return m_input_table.restoreState(numReads, cpos);
        }
        template<class T>
        inline bool getInTableData(unsigned int col, uint64_t row, T& v)
        {
            return m_input_table.getSpecificRowCol<T>(col, row, v);
        }
        inline row_number inTableNumberOfArguments()
        {
            return m_input_table.numberOfArguments();
        }
        template<class T>
        inline void setDirect(unsigned int col, T v)
        {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            m_table.set<T>(col, v);
        }
        inline void setGeoData(unsigned int col, const char *v, size_t l) {
            if (col >= m_types.size()) { m_exch->setException("Column does not exist"); return; }
            if (m_types[col].type != GEOMETRY) { m_exch->setException("Wrong column type (not geodata)"); return; }
            StringData arg;
            arg.length = l;
            arg.string = alloc_mword_aligned<char>(m_stack, l + 1);
            ::memcpy(arg.string, v, l);
            arg.string[l] = '\0';
            m_table.setGeo(col, arg, m_types[col].len);
        }
#endif
};

class SWIGVM {
    public:
        struct exception: std::exception {
            exception(const char *reason): m_reason(reason) { }
            virtual ~exception() throw() { }

            const char* what() const throw() { return m_reason.c_str(); }
            private:
                std::string m_reason;
        };

        SWIGVM() { }

        virtual ~SWIGVM() { }

        virtual void shutdown() {};
        virtual void destroy() {};
        virtual bool run() = 0;
        virtual std::string singleCall(single_call_function_id fn, const ExecutionGraph::ScriptDTO& args) = 0;
};


struct swig_undefined_single_call_exception: public std::exception 
{
    swig_undefined_single_call_exception(const std::string& fn): m_fn(fn) { }
    virtual ~swig_undefined_single_call_exception() throw() { }
    const std::string fn() const {return m_fn;}
    const char* what() const throw() {
        std::stringstream sb;
        sb << "Undefined in UDF: " << m_fn;
        return sb.str().c_str(); 
    }
 private:
    const std::string m_fn;
};


/* Forward declaretion of implementation classes */

$ifdef ENABLE_PYTHON_VM
class PythonVMImpl;
$endif

$ifdef ENABLE_R_VM
class RVMImpl;
$endif

#ifndef EXTERNAL_PROCESS
class ZMQVMImpl;
#endif

$ifdef ENABLE_GUILE_VM
class GuileVMImpl;
$endif

$ifdef ENABLE_JAVA_VM
class JavaVMImpl;
$endif

#ifdef EXTERNAL_PROCESS

$ifdef ENABLE_PYTHON_VM

class PythonVM: public SWIGVM {
    public:
        struct exception: SWIGVM::exception {
            exception(const char *reason): SWIGVM::exception(reason) { }
            virtual ~exception() throw() { }
        };

        PythonVM(bool checkOnly);
        virtual ~PythonVM() {};
        virtual void shutdown();
        virtual bool run();
        virtual std::string singleCall(single_call_function_id fn, const ExecutionGraph::ScriptDTO& args);
    private:
        PythonVMImpl *m_impl;
};

$endif
$ifdef ENABLE_R_VM

class RVM: public SWIGVM {
    public:
        struct exception: SWIGVM::exception {
            exception(const char *reason): SWIGVM::exception(reason) { }
            virtual ~exception() throw() { }
        };

        RVM(bool checkOnly);
        virtual ~RVM() {};
        virtual bool run();
        virtual void shutdown();
        virtual std::string singleCall(single_call_function_id fn, const ExecutionGraph::ScriptDTO& args);
    private:
        RVMImpl *m_impl;
};

$endif

$ifdef ENABLE_JAVA_VM
class JavaVMach: public SWIGVM {
    public:
        struct exception: SWIGVM::exception {
            exception(const char *reason): SWIGVM::exception(reason) { }
            virtual ~exception() throw() { }
        };
        JavaVMach(bool checkOnly);
        virtual ~JavaVMach() {}
        virtual void shutdown();
        virtual bool run();
        virtual std::string singleCall(single_call_function_id fn, const ExecutionGraph::ScriptDTO& args);
    private:
        JavaVMImpl *m_impl;
};
$endif

#else

class ZMQConnectionHandler;
ZMQConnectionHandler *create_internal_handler();
ZMQConnectionHandler *create_external_handler(const std::string &redirector_url);

class ZMQVM: public SWIGVM {
public:
    struct exception: std::exception {
    exception(const char *reason): m_reason(reason) { }
        virtual ~exception() throw() { }

        const char* what() const throw() { return m_reason.c_str(); }
    private:
        std::string m_reason;
    };

    ZMQVM(VMTYPE vm_type, ZMQConnectionHandler *handler, bool checkOnly, SWIGVM_params_t &params,
          const std::string& chroot_path,
          const std::string& exec_path,
          const std::vector<std::string>& ext_mount_paths,
          const std::string& query,
          const std::vector<std::string>& nsexec_env_var,
          bool isUDFPlugin);
protected:
    bool called_destroy;
    virtual ~ZMQVM();
public:
    virtual void destroy();
    virtual bool run();
    virtual std::string singleCall(single_call_function_id fn, const ExecutionGraph::ScriptDTO& args);

private:
    ZMQVMImpl *m_impl;
};
				   
#endif

} /* namespace SWIGVMContainers */
$endif
