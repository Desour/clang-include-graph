/**
 * src/include_graph_graphviz_printer.h
 *
 * Copyright (c) 2022-present Bartek Kryza <bkryza@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CLANG_INCLUDE_GRAPH_INCLUDE_GRAPH_GRAPHVIZ_PRINTER_H
#define CLANG_INCLUDE_GRAPH_INCLUDE_GRAPH_GRAPHVIZ_PRINTER_H

#include "include_graph_printer.h"

#include <boost/graph/graphviz.hpp>

#include <iostream>

namespace clang_include_graph {

namespace detail {
class label_writer {
public:
    label_writer(const include_graph_t::vertices_names_t &names,
        const path_printer_t &pp)
        : names_(names)
        , path_printer_{pp}
    {
    }
    template <typename Vertex>
    void operator()(std::ostream &out, const Vertex &v) const
    {
        out << "[label=\"" << path_printer_.print(names_.at(v)) << "\"]";
    }

private:
    const include_graph_t::vertices_names_t &names_;
    const path_printer_t &path_printer_;
};
}

class include_graph_graphviz_printer_t : public include_graph_printer_t {
public:
    using include_graph_printer_t::include_graph_printer_t;

    void print(const include_graph_t &graph) const override
    {
        detail::label_writer writer(graph.vertices_names, path_printer());

        boost::write_graphviz(std::cout, graph.graph, writer);
    }
};

} // namespace clang_include_graph

#endif // CLANG_INCLUDE_GRAPH_INCLUDE_GRAPH_GRAPHVIZ_PRINTER_H