#pragma once
#include <iostream>
#include <sstream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <toml.hpp>
namespace py = pybind11;

namespace pytomlpp {
// declarations
py::dict table_to_dict(const toml::table& t);
py::list array_to_list(const toml::array& a);

// implementations
py::list array_to_list(const toml::array& a) {
    py::list result;
    for(size_t i = 0; i < a.size(); i++) {
        const toml::node* value = a.get(i);
        if(value->type() == toml::node_type::integer) {
            const toml::value<int64_t>* integer_value = value->as_integer();
            const int64_t integer_v = integer_value->get();
            result.append(integer_v);
        } else if (value->type() == toml::node_type::floating_point) {
            const toml::value<double>* float_value = value->as_floating_point();
            const double float_v = float_value->get();
            result.append(float_v);
        } else if (value->type() == toml::node_type::string) {
            const toml::value<std::string>* string_value = value->as_string();
            const std::string string_v = string_value->get();
            result.append(string_v);
        } else if (value->type() == toml::node_type::boolean) {
            const toml::value<bool>* boolean_value = value->as_boolean();
            const bool bool_v = boolean_value->get();
            result.append(bool_v);
        } else if (value->type() == toml::node_type::none) {
            result.append(py::none());
        } else if (value->type() == toml::node_type::table) {
            const toml::table* table_value = value->as_table();
            py::dict d = table_to_dict(*table_value);
            result.append(d); 
        } else if (value->type() == toml::node_type::array) {
            const toml::array* array_value = value->as_array();
            py::list array_v = array_to_list(*array_value);
            result.append(array_v);
        }
        else {
            std::stringstream err_message;
            err_message << "cannot convert the type of this node to proper python types: " << value->type() << std::endl;
            std::string err_message_string = err_message.str();
            py::print(err_message_string);
            throw std::runtime_error(err_message_string);
        }
    }
    return result;
}

py::dict table_to_dict(const toml::table& t) {
    py::dict result;
    for(auto it: t) {
        auto key = it.first.c_str();
        const toml::node* value = &(it.second);
        if(value->type() == toml::node_type::integer) {
            const toml::value<int64_t>* integer_value = value->as_integer();
            const int64_t integer_v = integer_value->get();
            result[key] = integer_v;
        } else if (value->type() == toml::node_type::floating_point) {
            const toml::value<double>* float_value = value->as_floating_point();
            const double float_v = float_value->get();
            result[key] = float_v;
        } else if (value->type() == toml::node_type::string) {
            const toml::value<std::string>* string_value = value->as_string();
            const std::string string_v = string_value->get();
            result[key] = string_v;
        } else if (value->type() == toml::node_type::boolean) {
            const toml::value<bool>* boolean_value = value->as_boolean();
            const bool bool_v = boolean_value->get();
            result[key] = bool_v;
        } else if (value->type() == toml::node_type::none) {
            result[key] = py::none();
        } else if (value->type() == toml::node_type::table) {
            const toml::table* table_value = value->as_table();
            py::dict d = table_to_dict(*table_value);
            result[key] = d; 
        } else if (value->type() == toml::node_type::array) {
            const toml::array* array_value = value->as_array();
            py::list array_v = array_to_list(*array_value);
            result[key] = array_v;
        }
        else {
            std::stringstream err_message;
            err_message << "cannot convert the type of this node to proper python types: " << value->type() << std::endl;
            std::string err_message_string = err_message.str();
            py::print(err_message_string);
            throw std::runtime_error(err_message_string);
        }
    }
    return result;
}

}