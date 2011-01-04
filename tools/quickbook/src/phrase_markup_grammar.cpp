/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "grammar_impl.hpp"
#include "actions_class.hpp"
#include "utils.hpp"
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_assign_actor.hpp>
#include <boost/spirit/include/classic_clear_actor.hpp>
#include <boost/spirit/include/classic_if.hpp>

namespace quickbook
{
    namespace cl = boost::spirit::classic;

    struct phrase_markup_grammar_local
    {
        cl::rule<scanner>
                        image,
                        bold, italic, underline, teletype,
                        strikethrough, url, funcref, classref,
                        memberref, enumref, macroref, headerref, conceptref, globalref,
                        anchor, link,
                        source_mode_cpp, source_mode_python, source_mode_teletype,
                        quote, footnote, replaceable,
                        cond_phrase
                        ;
    };

    void quickbook_grammar::impl::init_phrase_markup()
    {
        using detail::var;

        phrase_markup_grammar_local& local = store_.create();

        phrase_symbol_rules.add
            ("?", &local.cond_phrase)
            ;

        local.cond_phrase =
                blank
            >>  macro_identifier                [actions.cond_phrase_pre]
            >>  actions.scoped_cond_phrase[extended_phrase]
            ;

        phrase_symbol_rules.add
            ("$", &local.image)
            ;

        local.image =
                blank                           [cl::clear_a(actions.attributes)]
            >>  cl::if_p(qbk_since(105u)) [
                        (+(
                            *cl::space_p
                        >>  +(cl::anychar_p - (cl::space_p | phrase_end | '['))
                        ))                       [cl::assign_a(actions.image_fileref)]
                    >>  hard_space
                    >>  *(
                            '['
                        >>  (*(cl::alnum_p | '_'))  [cl::assign_a(actions.attribute_name)]
                        >>  space
                        >>  (*(cl::anychar_p - (phrase_end | '[')))
                                                [actions.attribute]
                        >>  ']'
                        >>  space
                        )
                ].else_p [
                        (*(cl::anychar_p - phrase_end))
                                                [cl::assign_a(actions.image_fileref)]
                ]
            >>  cl::eps_p(']')                  [actions.image]
            ;
            
        phrase_symbol_rules.add
            ("@", &local.url)
            ;

        local.url =
                (*(cl::anychar_p -
                    (']' | hard_space)))  [actions.url_pre]
            >>  hard_space
            >>  phrase                          [actions.url_post]
            ;

        phrase_keyword_rules.add
            ("link", &local.link)
            ;

        local.link =
                space
            >>  (*(cl::anychar_p - (']' | hard_space)))
                                                [actions.link_pre]
            >>  hard_space
            >>  phrase                          [actions.link_post]
            ;

        phrase_symbol_rules.add
            ("#", &local.anchor)
            ;

        local.anchor =
                blank
            >>  (*(cl::anychar_p - phrase_end)) [actions.anchor]
            ;

        phrase_keyword_rules.add
            ("funcref", &local.funcref)
            ("classref", &local.classref)
            ("memberref", &local.memberref)
            ("enumref", &local.enumref)
            ("macroref", &local.macroref)
            ("headerref", &local.headerref)
            ("conceptref", &local.conceptref)
            ("globalref", &local.globalref)
            ;

        local.funcref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.funcref_pre]
            >>  hard_space
            >>  phrase                          [actions.funcref_post]
            ;

        local.classref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.classref_pre]
            >>  hard_space
            >>  phrase                          [actions.classref_post]
            ;

        local.memberref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.memberref_pre]
            >>  hard_space
            >>  phrase                          [actions.memberref_post]
            ;

        local.enumref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.enumref_pre]
            >>  hard_space
            >>  phrase                          [actions.enumref_post]
            ;

        local.macroref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.macroref_pre]
            >>  hard_space
            >>  phrase                          [actions.macroref_post]
            ;

        local.headerref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.headerref_pre]
            >>  hard_space
            >>  phrase                          [actions.headerref_post]
            ;

        local.conceptref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.conceptref_pre]
            >>  hard_space
            >>  phrase                          [actions.conceptref_post]
            ;

        local.globalref =
                space
            >>  (*(cl::anychar_p -
                    (']' | hard_space)))        [actions.globalref_pre]
            >>  hard_space
            >>  phrase                          [actions.globalref_post]
            ;

        phrase_symbol_rules.add
            ("*", &local.bold)
            ("'", &local.italic)
            ("_", &local.underline)
            ("^", &local.teletype)
            ("-", &local.strikethrough)
            ("\"", &local.quote)
            ("~", &local.replaceable)
            ;

        local.bold =
                blank                           [actions.bold_pre]
            >>  phrase                          [actions.bold_post]
            ;

        local.italic =
                blank                           [actions.italic_pre]
            >>  phrase                          [actions.italic_post]
            ;

        local.underline =
                blank                           [actions.underline_pre]
            >>  phrase                          [actions.underline_post]
            ;

        local.teletype =
                blank                           [actions.teletype_pre]
            >>  phrase                          [actions.teletype_post]
            ;

        local.strikethrough =
                blank                           [actions.strikethrough_pre]
            >>  phrase                          [actions.strikethrough_post]
            ;

        local.quote =
                blank                           [actions.quote_pre]
            >>  phrase                          [actions.quote_post]
            ;

        local.replaceable =
                blank                           [actions.replaceable_pre]
            >>  phrase                          [actions.replaceable_post]
            ;

        phrase_keyword_rules.add
            ("c++", &local.source_mode_cpp)
            ("python", &local.source_mode_python)
            ("teletype", &local.source_mode_teletype)
            ;
        
        local.source_mode_cpp = cl::eps_p [cl::assign_a(actions.source_mode, "c++")];
        local.source_mode_python = cl::eps_p [cl::assign_a(actions.source_mode, "python")];
        local.source_mode_teletype = cl::eps_p [cl::assign_a(actions.source_mode, "teletype")];

        phrase_keyword_rules.add
            ("footnote", &local.footnote)
            ;

        local.footnote =
                blank                           [actions.footnote_pre]
            >>  phrase                          [actions.footnote_post]
            ;
    }
}
