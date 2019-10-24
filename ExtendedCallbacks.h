#pragma once
#ifndef EXTENDED_CALLBACKS_H
#define EXTENDED_CALLBACKS_H

#include <iostream>
//#include <vector>
#include <set>
using namespace std;

// Author: Tojo Thomas tojohere@gmail.com
// Con unos cambios personales claro :)
template <class SenderType, class EventArgsType>
class Event{
public:
	// sender: por lo general la clase que dispara el evento
	// eventArg: informacion que se quiera pasar
	// context: por lo general, es el que se suscribe al evento (debe hacer el cast adecuado)
	typedef void (*EventHandler)(SenderType *sender, EventArgsType eventArg, void* context);

public:
	Event(SenderType *sender) : m_sender(sender) { }

	virtual ~Event() { }

	void operator +=(EventHandler handler) const {
		m_listeners.insert(ListenerRecord(handler));
	}

	void operator -=(EventHandler handler) const {
		typename set<ListenerRecord>::iterator it = m_listeners.find(ListenerRecord(handler));
		if (it != m_listeners.end() ) {
			m_listeners.erase(it);
		}
	}

	void Subscribe(EventHandler handler, void* context = 0) const {
		m_listeners.insert(ListenerRecord(handler, context));
	}

	void Unsubscribe(EventHandler handler, void* context = 0) const {
		typename set<ListenerRecord>::iterator it = m_listeners.find(ListenerRecord(handler,context));
		if (it != m_listeners.end()) {
			m_listeners.erase(it);
		}
	}

	void FireEvent(EventArgsType eventArg) {
	    typename set<ListenerRecord>::iterator it = m_listeners.begin();
		for(; it != m_listeners.end(); it++){
			it->handlerProc(m_sender, eventArg, it->handlerContext);
		}
	}

protected:
	class ListenerRecord
	{
	private:
		EventHandler handlerProc;
		void *handlerContext;
		ListenerRecord(EventHandler handler, void* context = 0)
			: handlerProc(handler), handlerContext(context){
		}
		friend class Event;
	public:
		bool operator<(const ListenerRecord & other) const {
			return handlerContext < other.handlerContext && handlerProc < other.handlerProc;
		}
	};

protected:
	mutable set<ListenerRecord>    m_listeners;
	mutable SenderType                *m_sender;
};



template <class Param>
struct Callback
{
public:
	// defino el tipo de la fucion para publicarlo
	// como no se puede usar directamente como parametro de funciones
	// tengo que sobrecargar un par de operadores :)
	typedef void (*FuncType)(Param);

	Callback<Param>(){};
	Callback<Param>(FuncType func) : m_func(func) {}

	void operator()(Param value){
		m_func(value);
	}

	Callback& operator=(FuncType func){
		m_func = func;
		return (*this);
	}

private:
	FuncType m_func;
};



// un callback generico que tambien recibe el objetivo como parametro
// esta claro que la funcion que implemente debera hacer el cast adecuado
template <class Param>
struct CppCallback
{
public:
	typedef void (*FuncType)(void*, Param);

	CppCallback<Param>(){};
	CppCallback<Param>(FuncType func) : m_func(func) {}


	void operator()(void* target, Param param){
		m_func(target, param);
	}

	CppCallback& operator=(FuncType func){
		m_func = func;
		return (*this);
	}

private:
	FuncType m_func;
};



template <class Target, class Param>
struct CppCallBack2
{
public:
	// defino el tipo de la fucion para publicarlo
	// como no se puede usar directamente como parametro de funciones
	// tengo que sobrecargar un par de operadores :)
	typedef void (*FuncType)(Target, Param);

	CppCallBack2<Target, Param>() : m_func(0) {};
	CppCallBack2<Target, Param>(FuncType func) : m_func(func) {}

	void operator()(Target target, Param param){
		m_func(target, param);
	}

	CppCallBack2& operator=(FuncType func){
		m_func = func;
		return (*this);
	}

private:
	FuncType m_func;
};



#endif
