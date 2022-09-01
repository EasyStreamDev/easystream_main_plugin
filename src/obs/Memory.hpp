/*
** Easystream EIP, 2021
** easystream_main_plugin
** File description:
** memory
*/

#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <string>
#include <obs.hpp>

void ___source_dummy_addref(obs_source_t *);
void ___weak_source_dummy_addref(obs_weak_source_t *);
void ___scene_dummy_addref(obs_scene_t *);
void ___sceneitem_dummy_addref(obs_sceneitem_t *);
void ___data_dummy_addref(obs_data_t *);
void ___data_array_dummy_addref(obs_data_array_t *);
void ___output_dummy_addref(obs_output_t *);
void ___data_item_dummy_addref(obs_data_item_t *);
void ___data_item_release(obs_data_item_t *);
void ___properties_dummy_addref(obs_properties_t *);

template <typename T>
T *GetCalldataPointer(const calldata_t *data, const char *name)
{
	void *ptr = nullptr;
	calldata_get_ptr(data, name, &ptr);
	return reinterpret_cast<T *>(ptr);
}

#endif /* !MEMORY_HPP_ */