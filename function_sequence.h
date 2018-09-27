//
//  function_sequence.h
//  ChrisTest
//
//  Created by Harward, Christopher on 9/18/18.
//  Copyright © 2018 Harward, Christopher. All rights reserved.
//

#pragma once

#include <utility>
#include <type_traits>

template<typename FirstFunction, typename... RemainingFunctions>
class function_sequence : public FirstFunction, public function_sequence<RemainingFunctions...>
{
public:
  template <typename FL, typename... RL>
  function_sequence(FL &&f, RL &&... r)
      : FirstFunction(std::forward<FL>(f)),
        function_sequence<RemainingFunctions...>(std::forward<RL>(r)...) {}

  template <typename... Args> auto operator()(Args &&... args) {
    return function_sequence<RemainingFunctions...>::operator()(
        FirstFunction::operator()(std::forward<Args>(args)...));
    }
};

template<typename Function>
class function_sequence<Function> : public Function
{
public:
    template<typename U>
    function_sequence(U&& u) : Function(std::forward<U>(u))
    {
    }
    
    using Function::operator();
};

template<typename... Functions>
auto make_function_sequence(Functions&&... functions){
    return function_sequence<std::remove_cv_t<std::remove_reference_t<Functions>>...>(
        std::forward<Functions>(functions)...);
}

