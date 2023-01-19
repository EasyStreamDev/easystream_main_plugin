/*
** EPITECH PROJECT, 2022
** easystream_main_plugin
** File description:
** Response
*/

#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include "../../Common.hpp"

namespace es {
    class Response {
        public:
            Response();
            ~Response();

            virtual void Resolve() = 0;

        protected:
        private:
    };
}

#endif /* !RESPONSE_HPP_ */
