/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexArray.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpeck <dpeck@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 15:08:24 by dpeck             #+#    #+#             */
/*   Updated: 2019/05/26 18:24:12 by dpeck            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {

private:
    unsigned int _rendererID;
    
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout);

    void bind() const;
    void unbind() const;

};

#endif