/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** TestResponses
*/

#ifndef TESTRESPONSES_HPP_
#define TESTRESPONSES_HPP_

#include "../Response.hpp"

namespace es {
    class TestResponses : public Response {
        public:
            TestResponses();
            ~TestResponses();

            virtual void Resolve() override;

        protected:
        private:
    };
}

#endif /* !TESTRESPONSES_HPP_ */
